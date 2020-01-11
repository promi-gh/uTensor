#include "RomTensor.hpp"
#include <cstdio>
namespace uTensor {
RomTensor::RomTensor(TensorShape _shape, ttype _type, const void* buffer) : 
    BufferTensor(_shape, _type, const_cast<void*>(buffer))
{ }

// TODO Need to fix the write/read selection functions in Handle
void* RomTensor::write(uint32_t linear_index) {
    //printf("[ERROR] Attempted write to ROM tensor, make sure it's declared const\n");
    //return nullptr;
    return BufferTensor::write(linear_index);
}

RomTensor::~RomTensor() { }
void RomTensor::resize(TensorShape new_shape) {
    printf("[ERROR] Attempted resize of ROM tensor\n");
}

// Returns floor of square root of x
int floorPerfSqrt(int x)
{
    // Base cases
    if (x == 0 || x == 1)
       return x;
    // Do Binary Search for floor(sqrt(x))
    int start = 1, end = x, ans;
    while (start <= end)
    {
        int mid = (start + end) / 2;
        // If x is a perfect square
        if (mid*mid == x)
            return mid;
        // Since we need floor, we update answer when mid*mid is
        // smaller than x, and move closer to sqrt(x)
        if (mid*mid < x)
        {
            start = mid + 1;
            ans = mid;
        }
        else // If mid*mid is greater than x
            end = mid-1;
    }
    // Did not find perfect square
    return -1;
}

DiagonalRomTensor::DiagonalRomTensor(TensorShape _shape, ttype _type, const void* buffer, size_t buffer_len) : RomTensor(_shape, _type, buffer) {
   if(_shape.num_dims() != 2){
    printf("[ERROR] Attempted to create diagonal Tensor with wrong number of dimensions\n");
    return;
   }
   uint16_t smaller_dim = (_shape[0] < _shape[1]) ? _shape[0] : _shape[1];
   if(buffer_len < smaller_dim){
    printf("[ERROR] Diagnoal Tensor size mismatch with buffer\n");
    return;
   }
}

DiagonalRomTensor::~DiagonalRomTensor() {}
void* DiagonalRomTensor::read(uint32_t linear_index) const {
    static const uint32_t zero = 0;
    int sqr = floorPerfSqrt(linear_index);
    if(sqr = linear_index/sqr){
        return RomTensor::read(sqr);
    } else {
        return (void*) &zero;
    }
}
// HACK TODO, REMOVE THIS
void* DiagonalRomTensor::write(uint32_t linear_index) {
    static uint32_t zero = 0;
    int sqr = floorPerfSqrt(linear_index);
    if(sqr = linear_index/sqr){
        return RomTensor::read(sqr);
    } else {
        return (void*) &zero;
    }
}

}
