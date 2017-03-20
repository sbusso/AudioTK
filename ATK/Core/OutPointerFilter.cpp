/**
 * \file OutPointerFilter.cpp
 */

#include "OutPointerFilter.h"

#include <algorithm>
#include <cstring>

namespace ATK
{
  template<typename DataType>
  OutPointerFilter<DataType>::OutPointerFilter(DataType* array, int channels, std::size_t size, bool interleaved)
  :TypedBaseFilter<DataType>(static_cast<int>(interleaved?size:channels), 0), offset(0), array(array), mysize(interleaved?channels:size), channels(static_cast<int>(interleaved?size:channels)), interleaved(interleaved)
  {
  }
  
  template<typename DataType>
  OutPointerFilter<DataType>::~OutPointerFilter()
  {
  }
  
  template<typename DataType>
  void OutPointerFilter<DataType>::set_pointer(DataType* array, std::size_t size)
  {
    this->array = array;
    mysize = size;
    offset = 0;
  }

  template<typename DataType>
  void OutPointerFilter<DataType>::process_impl(std::size_t size) const
  {
    if(!interleaved)
    {
      auto i = std::min(size, mysize - offset);
      for(unsigned int j = 0; j < channels; ++j)
      {
        memcpy(reinterpret_cast<void*>(&array[offset + (j * mysize)]), reinterpret_cast<const void*>(converted_inputs[j]), static_cast<size_t>(i) * sizeof(DataType));
      }
      for(; i < size; ++i)
      {
        for(unsigned int j = 0; j < channels; ++j)
        {
          array[offset + (j * mysize + i)] = 0;
        }
      }
    }
    else
    {
      std::size_t i;
      for(i = 0; i < size && (i + offset < mysize); ++i)
      {
        for(unsigned int j = 0; j < channels; ++j)
        {
          array[channels * offset + (j + i * channels)] = converted_inputs[j][i];
        }
      }
      for(; i < size; ++i)
      {
        for(unsigned int j = 0; j < channels; ++j)
        {
          array[channels * offset + (j + i * channels)] = 0;
        }
      }
    }
    
    offset += size;
  }
  
  template class OutPointerFilter<std::int16_t>;
  template class OutPointerFilter<std::int32_t>;
  template class OutPointerFilter<int64_t>;
  template class OutPointerFilter<float>;
  template class OutPointerFilter<double>;
}
