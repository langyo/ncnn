// Copyright 2021 Tencent
// SPDX-License-Identifier: BSD-3-Clause

static void convdw5x5s1_packn_fp16sa_rvv(const Mat& bottom_blob, Mat& top_blob, const Mat& kernel, const Mat& _bias, const Option& opt)
{
    const int packn = csrr_vlenb() / 2;
    const size_t vl = __riscv_vsetvl_e16m1(packn);

    int w = bottom_blob.w;

    int outw = top_blob.w;
    int outh = top_blob.h;

    const int group = bottom_blob.c;

    const __fp16* bias = _bias;

    #pragma omp parallel for num_threads(opt.num_threads)
    for (int g = 0; g < group; g++)
    {
        Mat out = top_blob.channel(g);

        vfloat16m1_t _bias0 = bias ? __riscv_vle16_v_f16m1(bias + g * packn, vl) : __riscv_vfmv_v_f_f16m1((__fp16)0.f, vl);

        const __fp16* k0 = kernel.row<const __fp16>(g);

        __fp16* outptr0 = out.row<__fp16>(0);
        __fp16* outptr1 = out.row<__fp16>(1);

        const Mat img0 = bottom_blob.channel(g);

        const __fp16* r0 = img0.row<const __fp16>(0);
        const __fp16* r1 = img0.row<const __fp16>(1);
        const __fp16* r2 = img0.row<const __fp16>(2);
        const __fp16* r3 = img0.row<const __fp16>(3);
        const __fp16* r4 = img0.row<const __fp16>(4);
        const __fp16* r5 = img0.row<const __fp16>(5);

        int i = 0;
        for (; i + 1 < outh; i += 2)
        {
            int j = 0;
            for (; j < outw; j++)
            {
                vfloat16m1_t _sum0 = _bias0;
                vfloat16m1_t _sum1 = _bias0;

                vfloat16m1_t _r00 = __riscv_vle16_v_f16m1(r0, vl);
                vfloat16m1_t _r01 = __riscv_vle16_v_f16m1(r0 + packn, vl);
                vfloat16m1_t _r02 = __riscv_vle16_v_f16m1(r0 + packn * 2, vl);
                vfloat16m1_t _r03 = __riscv_vle16_v_f16m1(r0 + packn * 3, vl);
                vfloat16m1_t _r04 = __riscv_vle16_v_f16m1(r0 + packn * 4, vl);

                vfloat16m1_t _k00 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k01 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k02 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k03 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k04 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k00, _r00, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k01, _r01, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k02, _r02, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k03, _r03, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k04, _r04, vl);

                vfloat16m1_t _r10 = __riscv_vle16_v_f16m1(r1, vl);
                vfloat16m1_t _r11 = __riscv_vle16_v_f16m1(r1 + packn, vl);
                vfloat16m1_t _r12 = __riscv_vle16_v_f16m1(r1 + packn * 2, vl);
                vfloat16m1_t _r13 = __riscv_vle16_v_f16m1(r1 + packn * 3, vl);
                vfloat16m1_t _r14 = __riscv_vle16_v_f16m1(r1 + packn * 4, vl);

                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k00, _r10, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k01, _r11, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k02, _r12, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k03, _r13, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k04, _r14, vl);

                vfloat16m1_t _k10 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k11 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k12 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k13 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k14 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k10, _r10, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k11, _r11, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k12, _r12, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k13, _r13, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k14, _r14, vl);

                vfloat16m1_t _r20 = __riscv_vle16_v_f16m1(r2, vl);
                vfloat16m1_t _r21 = __riscv_vle16_v_f16m1(r2 + packn, vl);
                vfloat16m1_t _r22 = __riscv_vle16_v_f16m1(r2 + packn * 2, vl);
                vfloat16m1_t _r23 = __riscv_vle16_v_f16m1(r2 + packn * 3, vl);
                vfloat16m1_t _r24 = __riscv_vle16_v_f16m1(r2 + packn * 4, vl);

                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k10, _r20, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k11, _r21, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k12, _r22, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k13, _r23, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k14, _r24, vl);

                vfloat16m1_t _k20 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k21 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k22 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k23 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k24 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k20, _r20, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k21, _r21, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k22, _r22, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k23, _r23, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k24, _r24, vl);

                vfloat16m1_t _r30 = __riscv_vle16_v_f16m1(r3, vl);
                vfloat16m1_t _r31 = __riscv_vle16_v_f16m1(r3 + packn, vl);
                vfloat16m1_t _r32 = __riscv_vle16_v_f16m1(r3 + packn * 2, vl);
                vfloat16m1_t _r33 = __riscv_vle16_v_f16m1(r3 + packn * 3, vl);
                vfloat16m1_t _r34 = __riscv_vle16_v_f16m1(r3 + packn * 4, vl);

                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k20, _r30, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k21, _r31, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k22, _r32, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k23, _r33, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k24, _r34, vl);

                vfloat16m1_t _k30 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k31 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k32 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k33 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k34 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k30, _r30, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k31, _r31, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k32, _r32, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k33, _r33, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k34, _r34, vl);

                vfloat16m1_t _r40 = __riscv_vle16_v_f16m1(r4, vl);
                vfloat16m1_t _r41 = __riscv_vle16_v_f16m1(r4 + packn, vl);
                vfloat16m1_t _r42 = __riscv_vle16_v_f16m1(r4 + packn * 2, vl);
                vfloat16m1_t _r43 = __riscv_vle16_v_f16m1(r4 + packn * 3, vl);
                vfloat16m1_t _r44 = __riscv_vle16_v_f16m1(r4 + packn * 4, vl);

                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k30, _r40, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k31, _r41, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k32, _r42, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k33, _r43, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k34, _r44, vl);

                vfloat16m1_t _k40 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k41 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k42 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k43 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k44 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 -= packn * 20;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k40, _r40, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k41, _r41, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k42, _r42, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k43, _r43, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k44, _r44, vl);

                vfloat16m1_t _r50 = __riscv_vle16_v_f16m1(r5, vl);
                vfloat16m1_t _r51 = __riscv_vle16_v_f16m1(r5 + packn, vl);
                vfloat16m1_t _r52 = __riscv_vle16_v_f16m1(r5 + packn * 2, vl);
                vfloat16m1_t _r53 = __riscv_vle16_v_f16m1(r5 + packn * 3, vl);
                vfloat16m1_t _r54 = __riscv_vle16_v_f16m1(r5 + packn * 4, vl);

                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k40, _r50, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k41, _r51, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k42, _r52, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k43, _r53, vl);
                _sum1 = __riscv_vfmacc_vv_f16m1(_sum1, _k44, _r54, vl);

                __riscv_vse16_v_f16m1(outptr0, _sum0, vl);
                __riscv_vse16_v_f16m1(outptr1, _sum1, vl);

                outptr0 += packn;
                outptr1 += packn;

                r0 += packn;
                r1 += packn;
                r2 += packn;
                r3 += packn;
                r4 += packn;
                r5 += packn;
            }

            r0 += 4 * packn + w * packn;
            r1 += 4 * packn + w * packn;
            r2 += 4 * packn + w * packn;
            r3 += 4 * packn + w * packn;
            r4 += 4 * packn + w * packn;
            r5 += 4 * packn + w * packn;

            outptr0 += outw * packn;
            outptr1 += outw * packn;
        }
        for (; i < outh; i++)
        {
            int j = 0;
            for (; j < outw; j++)
            {
                vfloat16m1_t _sum0 = _bias0;

                vfloat16m1_t _r00 = __riscv_vle16_v_f16m1(r0, vl);
                vfloat16m1_t _r01 = __riscv_vle16_v_f16m1(r0 + packn, vl);
                vfloat16m1_t _r02 = __riscv_vle16_v_f16m1(r0 + packn * 2, vl);
                vfloat16m1_t _r03 = __riscv_vle16_v_f16m1(r0 + packn * 3, vl);
                vfloat16m1_t _r04 = __riscv_vle16_v_f16m1(r0 + packn * 4, vl);

                vfloat16m1_t _k00 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k01 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k02 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k03 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k04 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k00, _r00, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k01, _r01, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k02, _r02, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k03, _r03, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k04, _r04, vl);

                vfloat16m1_t _r10 = __riscv_vle16_v_f16m1(r1, vl);
                vfloat16m1_t _r11 = __riscv_vle16_v_f16m1(r1 + packn, vl);
                vfloat16m1_t _r12 = __riscv_vle16_v_f16m1(r1 + packn * 2, vl);
                vfloat16m1_t _r13 = __riscv_vle16_v_f16m1(r1 + packn * 3, vl);
                vfloat16m1_t _r14 = __riscv_vle16_v_f16m1(r1 + packn * 4, vl);

                vfloat16m1_t _k10 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k11 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k12 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k13 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k14 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k10, _r10, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k11, _r11, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k12, _r12, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k13, _r13, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k14, _r14, vl);

                vfloat16m1_t _r20 = __riscv_vle16_v_f16m1(r2, vl);
                vfloat16m1_t _r21 = __riscv_vle16_v_f16m1(r2 + packn, vl);
                vfloat16m1_t _r22 = __riscv_vle16_v_f16m1(r2 + packn * 2, vl);
                vfloat16m1_t _r23 = __riscv_vle16_v_f16m1(r2 + packn * 3, vl);
                vfloat16m1_t _r24 = __riscv_vle16_v_f16m1(r2 + packn * 4, vl);

                vfloat16m1_t _k20 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k21 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k22 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k23 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k24 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k20, _r20, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k21, _r21, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k22, _r22, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k23, _r23, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k24, _r24, vl);

                vfloat16m1_t _r30 = __riscv_vle16_v_f16m1(r3, vl);
                vfloat16m1_t _r31 = __riscv_vle16_v_f16m1(r3 + packn, vl);
                vfloat16m1_t _r32 = __riscv_vle16_v_f16m1(r3 + packn * 2, vl);
                vfloat16m1_t _r33 = __riscv_vle16_v_f16m1(r3 + packn * 3, vl);
                vfloat16m1_t _r34 = __riscv_vle16_v_f16m1(r3 + packn * 4, vl);

                vfloat16m1_t _k30 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k31 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k32 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k33 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k34 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k30, _r30, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k31, _r31, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k32, _r32, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k33, _r33, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k34, _r34, vl);

                vfloat16m1_t _r40 = __riscv_vle16_v_f16m1(r4, vl);
                vfloat16m1_t _r41 = __riscv_vle16_v_f16m1(r4 + packn, vl);
                vfloat16m1_t _r42 = __riscv_vle16_v_f16m1(r4 + packn * 2, vl);
                vfloat16m1_t _r43 = __riscv_vle16_v_f16m1(r4 + packn * 3, vl);
                vfloat16m1_t _r44 = __riscv_vle16_v_f16m1(r4 + packn * 4, vl);

                vfloat16m1_t _k40 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k41 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k42 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k43 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k44 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 -= packn * 20;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k40, _r40, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k41, _r41, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k42, _r42, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k43, _r43, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k44, _r44, vl);

                __riscv_vse16_v_f16m1(outptr0, _sum0, vl);

                outptr0 += packn;

                r0 += packn;
                r1 += packn;
                r2 += packn;
                r3 += packn;
                r4 += packn;
            }

            r0 += 4 * packn;
            r1 += 4 * packn;
            r2 += 4 * packn;
            r3 += 4 * packn;
            r4 += 4 * packn;
        }
    }
}

static void convdw5x5s2_packn_fp16sa_rvv(const Mat& bottom_blob, Mat& top_blob, const Mat& kernel, const Mat& _bias, const Option& opt)
{
    const int packn = csrr_vlenb() / 2;
    const size_t vl = __riscv_vsetvl_e16m1(packn);

    int w = bottom_blob.w;

    int outw = top_blob.w;
    int outh = top_blob.h;

    const int group = bottom_blob.c;

    const int tailstep = (w - 2 * outw + w) * packn;

    const __fp16* bias = _bias;

    #pragma omp parallel for num_threads(opt.num_threads)
    for (int g = 0; g < group; g++)
    {
        Mat out = top_blob.channel(g);

        vfloat16m1_t _bias0 = bias ? __riscv_vle16_v_f16m1(bias + g * packn, vl) : __riscv_vfmv_v_f_f16m1((__fp16)0.f, vl);

        const __fp16* k0 = kernel.row<const __fp16>(g);

        __fp16* outptr0 = out.row<__fp16>(0);

        const Mat img0 = bottom_blob.channel(g);

        const __fp16* r0 = img0.row<const __fp16>(0);
        const __fp16* r1 = img0.row<const __fp16>(1);
        const __fp16* r2 = img0.row<const __fp16>(2);
        const __fp16* r3 = img0.row<const __fp16>(3);
        const __fp16* r4 = img0.row<const __fp16>(4);

        int i = 0;
        for (; i < outh; i++)
        {
            int j = 0;
            for (; j < outw; j++)
            {
                vfloat16m1_t _sum0 = _bias0;

                vfloat16m1_t _r00 = __riscv_vle16_v_f16m1(r0, vl);
                vfloat16m1_t _r01 = __riscv_vle16_v_f16m1(r0 + packn, vl);
                vfloat16m1_t _r02 = __riscv_vle16_v_f16m1(r0 + packn * 2, vl);
                vfloat16m1_t _r03 = __riscv_vle16_v_f16m1(r0 + packn * 3, vl);
                vfloat16m1_t _r04 = __riscv_vle16_v_f16m1(r0 + packn * 4, vl);

                vfloat16m1_t _k00 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k01 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k02 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k03 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k04 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k00, _r00, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k01, _r01, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k02, _r02, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k03, _r03, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k04, _r04, vl);

                vfloat16m1_t _r10 = __riscv_vle16_v_f16m1(r1, vl);
                vfloat16m1_t _r11 = __riscv_vle16_v_f16m1(r1 + packn, vl);
                vfloat16m1_t _r12 = __riscv_vle16_v_f16m1(r1 + packn * 2, vl);
                vfloat16m1_t _r13 = __riscv_vle16_v_f16m1(r1 + packn * 3, vl);
                vfloat16m1_t _r14 = __riscv_vle16_v_f16m1(r1 + packn * 4, vl);

                vfloat16m1_t _k10 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k11 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k12 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k13 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k14 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k10, _r10, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k11, _r11, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k12, _r12, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k13, _r13, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k14, _r14, vl);

                vfloat16m1_t _r20 = __riscv_vle16_v_f16m1(r2, vl);
                vfloat16m1_t _r21 = __riscv_vle16_v_f16m1(r2 + packn, vl);
                vfloat16m1_t _r22 = __riscv_vle16_v_f16m1(r2 + packn * 2, vl);
                vfloat16m1_t _r23 = __riscv_vle16_v_f16m1(r2 + packn * 3, vl);
                vfloat16m1_t _r24 = __riscv_vle16_v_f16m1(r2 + packn * 4, vl);

                vfloat16m1_t _k20 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k21 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k22 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k23 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k24 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k20, _r20, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k21, _r21, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k22, _r22, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k23, _r23, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k24, _r24, vl);

                vfloat16m1_t _r30 = __riscv_vle16_v_f16m1(r3, vl);
                vfloat16m1_t _r31 = __riscv_vle16_v_f16m1(r3 + packn, vl);
                vfloat16m1_t _r32 = __riscv_vle16_v_f16m1(r3 + packn * 2, vl);
                vfloat16m1_t _r33 = __riscv_vle16_v_f16m1(r3 + packn * 3, vl);
                vfloat16m1_t _r34 = __riscv_vle16_v_f16m1(r3 + packn * 4, vl);

                vfloat16m1_t _k30 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k31 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k32 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k33 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k34 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 += packn * 5;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k30, _r30, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k31, _r31, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k32, _r32, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k33, _r33, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k34, _r34, vl);

                vfloat16m1_t _r40 = __riscv_vle16_v_f16m1(r4, vl);
                vfloat16m1_t _r41 = __riscv_vle16_v_f16m1(r4 + packn, vl);
                vfloat16m1_t _r42 = __riscv_vle16_v_f16m1(r4 + packn * 2, vl);
                vfloat16m1_t _r43 = __riscv_vle16_v_f16m1(r4 + packn * 3, vl);
                vfloat16m1_t _r44 = __riscv_vle16_v_f16m1(r4 + packn * 4, vl);

                vfloat16m1_t _k40 = __riscv_vle16_v_f16m1(k0, vl);
                vfloat16m1_t _k41 = __riscv_vle16_v_f16m1(k0 + packn, vl);
                vfloat16m1_t _k42 = __riscv_vle16_v_f16m1(k0 + packn * 2, vl);
                vfloat16m1_t _k43 = __riscv_vle16_v_f16m1(k0 + packn * 3, vl);
                vfloat16m1_t _k44 = __riscv_vle16_v_f16m1(k0 + packn * 4, vl);
                k0 -= packn * 20;

                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k40, _r40, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k41, _r41, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k42, _r42, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k43, _r43, vl);
                _sum0 = __riscv_vfmacc_vv_f16m1(_sum0, _k44, _r44, vl);

                __riscv_vse16_v_f16m1(outptr0, _sum0, vl);

                outptr0 += packn;

                r0 += packn * 2;
                r1 += packn * 2;
                r2 += packn * 2;
                r3 += packn * 2;
                r4 += packn * 2;
            }

            r0 += tailstep;
            r1 += tailstep;
            r2 += tailstep;
            r3 += tailstep;
            r4 += tailstep;
        }
    }
}
