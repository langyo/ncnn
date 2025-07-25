// Copyright 2021 Tencent
// SPDX-License-Identifier: BSD-3-Clause

#include "pass_ncnn.h"

namespace pnnx {

namespace ncnn {

class nn_Conv3d : public GraphRewriterPass
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
3 2
pnnx.Input              input       0 1 input
nn.Conv3d               op_0        1 1 input out in_channels=%in_channels out_channels=%out_channels kernel_size=%kernel_size stride=%stride padding_mode=zeros padding=%padding dilation=%dilation groups=1 bias=%bias @weight @bias
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* type_str() const
    {
        return "Convolution3D";
    }

    const char* name_str() const
    {
        return "conv3d";
    }

    void write(Operator* op, const std::map<std::string, Parameter>& captured_params, const std::map<std::string, Attribute>& captured_attrs) const
    {
        op->params["0"] = captured_params.at("out_channels");
        op->params["1"] = captured_params.at("kernel_size").ai[2];
        op->params["11"] = captured_params.at("kernel_size").ai[1];
        op->params["21"] = captured_params.at("kernel_size").ai[0];
        op->params["2"] = captured_params.at("dilation").ai[2];
        op->params["12"] = captured_params.at("dilation").ai[1];
        op->params["22"] = captured_params.at("dilation").ai[0];
        op->params["3"] = captured_params.at("stride").ai[2];
        op->params["13"] = captured_params.at("stride").ai[1];
        op->params["23"] = captured_params.at("stride").ai[0];
        if (captured_params.at("padding").type == 4)
        {
            if (captured_params.at("padding").s == "same")
                op->params["4"] = -233;
            else if (captured_params.at("padding").s == "valid")
                op->params["4"] = 0;
        }
        else
        {
            op->params["4"] = captured_params.at("padding").ai[2];
            op->params["14"] = captured_params.at("padding").ai[1];
            op->params["24"] = captured_params.at("padding").ai[0];
        }
        op->params["5"] = captured_params.at("bias").b ? 1 : 0;
        op->params["6"] = captured_attrs.at("op_0.weight").elemcount();

        op->attrs["0"] = Attribute();
        op->attrs["0"].data = {0, 0, 0, 0};
        op->attrs["1"] = captured_attrs.at("op_0.weight");
        if (captured_params.at("bias").b)
            op->attrs["2"] = captured_attrs.at("op_0.bias");
    }
};

class nn_Conv3d_1 : public GraphRewriterPass
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
3 2
pnnx.Input              input       0 1 input
nn.Conv3d               op_0        1 1 input out in_channels=%in_channels out_channels=%out_channels kernel_size=%kernel_size stride=%stride padding_mode=zeros padding=%padding dilation=%dilation groups=%groups bias=%bias @weight @bias
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* type_str() const
    {
        return "ConvolutionDepthWise3D";
    }

    const char* name_str() const
    {
        return "convdw3d";
    }

    void write(Operator* op, const std::map<std::string, Parameter>& captured_params, const std::map<std::string, Attribute>& captured_attrs) const
    {
        op->params["0"] = captured_params.at("out_channels");
        op->params["1"] = captured_params.at("kernel_size").ai[2];
        op->params["11"] = captured_params.at("kernel_size").ai[1];
        op->params["21"] = captured_params.at("kernel_size").ai[0];
        op->params["2"] = captured_params.at("dilation").ai[2];
        op->params["12"] = captured_params.at("dilation").ai[1];
        op->params["22"] = captured_params.at("dilation").ai[0];
        op->params["3"] = captured_params.at("stride").ai[2];
        op->params["13"] = captured_params.at("stride").ai[1];
        op->params["23"] = captured_params.at("stride").ai[0];
        if (captured_params.at("padding").type == 4)
        {
            if (captured_params.at("padding").s == "same")
                op->params["4"] = -233;
            else if (captured_params.at("padding").s == "valid")
                op->params["4"] = 0;
        }
        else
        {
            op->params["4"] = captured_params.at("padding").ai[2];
            op->params["14"] = captured_params.at("padding").ai[1];
            op->params["24"] = captured_params.at("padding").ai[0];
        }
        op->params["5"] = captured_params.at("bias").b ? 1 : 0;
        op->params["6"] = captured_attrs.at("op_0.weight").elemcount();
        op->params["7"] = captured_params.at("groups");

        op->attrs["0"] = Attribute();
        op->attrs["0"].data = {0, 0, 0, 0};
        op->attrs["1"] = captured_attrs.at("op_0.weight");
        if (captured_params.at("bias").b)
            op->attrs["2"] = captured_attrs.at("op_0.bias");
    }
};

class nn_Conv3d_2 : public GraphRewriterPass
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
3 2
pnnx.Input              input       0 1 input
nn.Conv3d               op_0        1 1 input out in_channels=%in_channels out_channels=%out_channels kernel_size=%kernel_size stride=%stride padding_mode=%padding_mode padding=%padding dilation=%dilation groups=1 bias=%bias @weight @bias
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* replace_pattern_graph() const
    {
        return R"PNNXIR(7767517
4 3
pnnx.Input              input       0 1 input
Padding                 pad         1 1 input a
Convolution3D           conv        1 1 a out
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    bool match(const std::map<std::string, const Operator*>& matched_operators, const std::map<std::string, Parameter>& captured_params, const std::map<std::string, Attribute>& /*captured_attrs*/) const
    {
        const std::string& padding_mode = captured_params.at("padding_mode").s;
        if (padding_mode == "zeros")
            return false;

        const Operator* conv = matched_operators.at("op_0");
        if (conv->params.at("padding").type == 4 && conv->params.at("padding").s == "same")
        {
            const std::vector<int> input_shape = conv->inputs[0]->shape;
            if (input_shape.size() != 4 && input_shape.size() != 5)
            {
                fprintf(stderr, "can not resolve pads without shape\n");
                return false;
            }
        }

        return true;
    }

    void write(const std::map<std::string, Operator*>& ops, const std::map<std::string, Parameter>& captured_params, const std::map<std::string, Attribute>& captured_attrs) const
    {
        std::vector<int> padding;
        if (captured_params.at("padding").type == 4)
        {
            if (captured_params.at("padding").s == "same")
            {
                // resolve pads
                const std::vector<int> input_shape = ops.at("pad")->inputs[0]->shape;
                const int w = input_shape[input_shape.size() - 1];
                const int h = input_shape[input_shape.size() - 2];
                const int d = input_shape[input_shape.size() - 3];
                const int kernel_w = captured_params.at("kernel_size").ai[2];
                const int kernel_h = captured_params.at("kernel_size").ai[1];
                const int kernel_d = captured_params.at("kernel_size").ai[0];
                const int dilation_w = captured_params.at("dilation").ai[2];
                const int dilation_h = captured_params.at("dilation").ai[1];
                const int dilation_d = captured_params.at("dilation").ai[0];
                const int stride_w = captured_params.at("stride").ai[2];
                const int stride_h = captured_params.at("stride").ai[1];
                const int stride_d = captured_params.at("stride").ai[0];

                const int kernel_extent_w = dilation_w * (kernel_w - 1) + 1;
                const int kernel_extent_h = dilation_h * (kernel_h - 1) + 1;
                const int kernel_extent_d = dilation_d * (kernel_d - 1) + 1;

                int wpad = kernel_extent_w + (w - 1) / stride_w * stride_w - w;
                int hpad = kernel_extent_h + (h - 1) / stride_h * stride_h - h;
                int dpad = kernel_extent_d + (d - 1) / stride_d * stride_d - d;

                padding = std::vector<int>{hpad / 2, hpad - hpad / 2, wpad / 2, wpad - wpad / 2, dpad / 2, dpad - dpad / 2};
            }
            else if (captured_params.at("padding").s == "valid")
            {
                padding = std::vector<int>{0, 0, 0, 0, 0, 0};
            }
        }
        else
        {
            int dpad = captured_params.at("padding").ai[0];
            int hpad = captured_params.at("padding").ai[1];
            int wpad = captured_params.at("padding").ai[2];
            padding = std::vector<int>{hpad, hpad, wpad, wpad, dpad, dpad};
        }

        ops.at("pad")->params["0"] = padding[0];
        ops.at("pad")->params["1"] = padding[1];
        ops.at("pad")->params["2"] = padding[2];
        ops.at("pad")->params["3"] = padding[3];
        ops.at("pad")->params["7"] = padding[4];
        ops.at("pad")->params["8"] = padding[5];

        std::string padding_mode = captured_params.at("padding_mode").s;
        if (padding_mode == "reflect")
        {
            ops.at("pad")->params["4"] = 2; // type=reflect
        }
        else if (padding_mode == "replicate")
        {
            ops.at("pad")->params["4"] = 1; // type=replicate
        }
        else
        {
            fprintf(stderr, "unsupported padding_mode %s\n", padding_mode.c_str());
        }

        ops.at("conv")->params["0"] = captured_params.at("out_channels");
        ops.at("conv")->params["1"] = captured_params.at("kernel_size").ai[2];
        ops.at("conv")->params["11"] = captured_params.at("kernel_size").ai[1];
        ops.at("conv")->params["21"] = captured_params.at("kernel_size").ai[0];
        ops.at("conv")->params["2"] = captured_params.at("dilation").ai[2];
        ops.at("conv")->params["12"] = captured_params.at("dilation").ai[1];
        ops.at("conv")->params["22"] = captured_params.at("dilation").ai[0];
        ops.at("conv")->params["3"] = captured_params.at("stride").ai[2];
        ops.at("conv")->params["13"] = captured_params.at("stride").ai[1];
        ops.at("conv")->params["23"] = captured_params.at("stride").ai[0];
        ops.at("conv")->params["4"] = 0;
        ops.at("conv")->params["14"] = 0;
        ops.at("conv")->params["24"] = 0;
        ops.at("conv")->params["5"] = captured_params.at("bias").b ? 1 : 0;
        ops.at("conv")->params["6"] = captured_attrs.at("op_0.weight").elemcount();
        ops.at("conv")->params["7"] = captured_params.find("groups") != captured_params.end() ? captured_params.at("groups") : 1;

        ops.at("conv")->attrs["0"] = Attribute();
        ops.at("conv")->attrs["0"].data = {0, 0, 0, 0};
        ops.at("conv")->attrs["1"] = captured_attrs.at("op_0.weight");
        if (captured_params.at("bias").b)
            ops.at("conv")->attrs["2"] = captured_attrs.at("op_0.bias");
    }
};

class nn_Conv3d_3 : public nn_Conv3d_2
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
3 2
pnnx.Input              input       0 1 input
nn.Conv3d               op_0        1 1 input out in_channels=%in_channels out_channels=%out_channels kernel_size=%kernel_size stride=%stride padding_mode=%padding_mode padding=%padding dilation=%dilation groups=%groups bias=%bias @weight @bias
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* replace_pattern_graph() const
    {
        return R"PNNXIR(7767517
4 3
pnnx.Input              input       0 1 input
Padding                 pad         1 1 input a
ConvolutionDepthWise3D  conv        1 1 a out
pnnx.Output             output      1 0 out
)PNNXIR";
    }
};

REGISTER_GLOBAL_PNNX_NCNN_GRAPH_REWRITER_PASS(nn_Conv3d, 20)
REGISTER_GLOBAL_PNNX_NCNN_GRAPH_REWRITER_PASS(nn_Conv3d_1, 21)
REGISTER_GLOBAL_PNNX_NCNN_GRAPH_REWRITER_PASS(nn_Conv3d_2, 22)
REGISTER_GLOBAL_PNNX_NCNN_GRAPH_REWRITER_PASS(nn_Conv3d_3, 23)

} // namespace ncnn

} // namespace pnnx
