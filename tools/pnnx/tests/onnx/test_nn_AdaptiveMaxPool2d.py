# Copyright 2025 Tencent
# SPDX-License-Identifier: BSD-3-Clause

import torch
import torch.nn as nn
import torch.nn.functional as F
from packaging import version

class Model(nn.Module):
    def __init__(self):
        super(Model, self).__init__()

        self.pool_0 = nn.AdaptiveMaxPool2d(output_size=(7,6), return_indices=True)
        self.pool_1 = nn.AdaptiveMaxPool2d(output_size=1)
        self.pool_2 = nn.AdaptiveMaxPool2d(output_size=(None,3))
        self.pool_3 = nn.AdaptiveMaxPool2d(output_size=(2,None), return_indices=True)

    def forward(self, x):
        out0, indices0 = self.pool_0(x)
        out1 = self.pool_1(x)
        if version.parse(torch.__version__) < version.parse('1.10'):
            return out0, indices0, out1

        out2 = self.pool_2(x)
        out3, indices3 = self.pool_3(x)
        return out0, indices0, out1, out2, out3, indices3

def test():
    net = Model()
    net.eval()

    torch.manual_seed(0)
    x = torch.rand(1, 128, 14, 12)

    a = net(x)

    # export onnx
    torch.onnx.export(net, (x,), "test_nn_AdaptiveMaxPool2d.onnx")

    # onnx to pnnx
    import os
    os.system("../../src/pnnx test_nn_AdaptiveMaxPool2d.onnx inputshape=[1,128,14,12]")

    # pnnx inference
    import test_nn_AdaptiveMaxPool2d_pnnx
    b = test_nn_AdaptiveMaxPool2d_pnnx.test_inference()

    for a0, b0 in zip(a, b):
        if not torch.equal(a0, b0):
            return False
    return True

if __name__ == "__main__":
    if test():
        exit(0)
    else:
        exit(1)
