/**
 * @file
 * @author DeepLink
 * @copyright  (c) 2023, DeepLink.
 */

#include <diopi/functions.h>

#include "../common/acloprunner.hpp"

namespace impl {
namespace ascend {
extern "C" {
DIOPI_API diopiError_t diopiUpsampleNearest(diopiContextHandle_t ctx, diopiTensorHandle_t out, diopiConstTensorHandle_t input, diopiSize_t size) {
    /*diopiSize_t inputSize;
    diopiGetTensorShape(input, &inputSize);
    std::vector<int64_t> outputSizeVec({inputSize.data[0], inputSize.data[1], size.data[0], size.data[1]});
    diopiSize_t outputSize(outputSizeVec.data(), outputSizeVec.size());
    const void* data;
    diopiGetTensorDataConst(input, &data);
    int64_t elemsize, numel;
    diopiDtype_t dtype;
    diopiGetTensorElemSize(input, &elemsize);
    diopiGetTensorNumel(input, &numel);
    diopiGetTensorDtype(input, &dtype);*/
    AclOpRunner<2, 1>("ResizeNearestNeighborV2", ctx)
        .addInput(input)
        .addConstInput(size, diopi_dtype_int32)
        .setAttr("align_corners", false)
        .setAttr("half_pixel_centers", false)
        .addOutput(out)
        .run();
    return diopiSuccess;
}

DIOPI_API diopiError_t diopiUpsampleNearestBackward(diopiContextHandle_t ctx, diopiTensorHandle_t gradInput, diopiConstTensorHandle_t gradOutput,
                                                    diopiSize_t outSize, diopiSize_t inSize) {
    auto gradOutputCopy = contiguous(ctx, gradOutput);
    std::vector<int64_t> outputSizeVec({inSize.data[2], inSize.data[3]});
    diopiSize_t outputSize(outputSizeVec.data(), outputSizeVec.size());
    AclOpRunner<2, 1>("ResizeNearestNeighborV2Grad", ctx)
        .addInput(gradOutputCopy)
        .addConstInput(outputSize, diopi_dtype_int32)
        .setAttr("align_corners", false)
        .setAttr("half_pixel_centers", false)
        .addOutput(gradInput)
        .run();
    return diopiSuccess;
}
}

}  // namespace ascend
}  // namespace impl
