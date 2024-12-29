/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template <typename DType, typename LType>
class DataLoader
{
public:
    class Iterator; // forward declaration for class Iterator

private:
    Dataset<DType, LType> *ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int nbatch;
    ulong_tensor item_indices;
    int m_seed;
    int last_remainder;

public:
    DataLoader(Dataset<DType, LType> *ptr_dataset,
               int batch_size, bool shuffle = true,
               bool drop_last = false, int seed = -1)
        : ptr_dataset(ptr_dataset),
          batch_size(batch_size),
          shuffle(shuffle),
          m_seed(seed)
    {
        this->drop_last = drop_last;
        nbatch = ptr_dataset->len() / batch_size;
        last_remainder = ptr_dataset->len() % batch_size;
        item_indices = xt::arange(0, ptr_dataset->len());
        // if (shuffle && m_seed < 0)
        // {
        //     xt::random::shuffle(item_indices);
        // }
        // else if (shuffle && m_seed >= 0)
        // {
        //     xt::random::seed(m_seed);
        //     xt::random::shuffle(item_indices);
        // }
    }
    virtual ~DataLoader() {}

    // New method: from V2: begin
    int get_batch_size() { return batch_size; }
    int get_sample_count() { return ptr_dataset->len(); }
    int get_total_batch() { return int(ptr_dataset->len() / batch_size); }

    // New method: from V2: end
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
public:
    Iterator begin()
    {
        // YOUR CODE IS HERE
        if (shuffle && m_seed < 0)
        {
            xt::random::shuffle(item_indices);
        }
        else if (shuffle && m_seed >= 0)
        {
            xt::random::seed(m_seed);
            xt::random::shuffle(item_indices);
        }
        return Iterator(this, 0);
    }
    Iterator end()
    {
        // YOUR CODE IS HERE
        return Iterator(this, ptr_dataset->len()/batch_size);
    }

    // BEGIN of Iterator

    // YOUR CODE IS HERE: to define iterator
    class Iterator
  {
  private:
    int cursor;
    DataLoader<DType, LType> *ptr_to_dataloader;

  public:
    // TODO implement contructor
    Iterator(DataLoader<DType, LType> *dptr, int index)
    {
      this->ptr_to_dataloader = dptr;
      this->cursor = index;
    }
    Iterator &operator=(const Iterator &iterator)
    {
      // TODO implement
      this->cursor = iterator.cursor;
      if (this->ptr_to_dataloader != iterator.ptr_to_dataloader)
      {
        this->ptr_to_dataloader = iterator.ptr_to_dataloader;
      }
      return *this;
    }

    Iterator &operator++()
    {
      // TODO implement
      this->cursor++;
      return *this;
    }

    Iterator operator++(int)
    {
      // TODO implement
      Iterator olditer = *this;
      ++(*this);
      return olditer;
    }
    Iterator operator--()
    {
      this->cursor--;
      if (cursor < 0)
        throw std::out_of_range("Index is out of range!");
      return *this;
    }
    bool operator==(const Iterator &other) const
    {
      return (this->cursor == other.cursor);
    }
    bool operator!=(const Iterator &other) const
    {
      // TODO implement
      return (this->cursor != other.cursor);
    }

    Batch<DType, LType> operator*() const{
      auto dshape = ptr_to_dataloader->ptr_dataset->get_data_shape();
      auto lshape = ptr_to_dataloader->ptr_dataset->get_label_shape();
      bool lshapeCheck;
      if(lshape.empty()) lshapeCheck = true;
      else lshapeCheck = false;
      xt::xarray<DType> data;
      xt::xarray<LType> label;
      // Neu nhu so phan tu < batch size
      if (this->ptr_to_dataloader->nbatch == 0)
      {
        if (this->ptr_to_dataloader->drop_last)
        {
          data = xt::xarray<DType>::from_shape({});
          label = xt::xarray<LType>::from_shape({});
          return Batch<DType, LType>(data, label);
        }
        else
        {
          dshape[0] = this->ptr_to_dataloader->last_remainder;
          lshape[0] = this->ptr_to_dataloader->last_remainder;
          data = xt::xarray<DType>::from_shape(dshape);
          label = xt::xarray<LType>::from_shape(lshape);
          for (int i = 0; i < this->ptr_to_dataloader->last_remainder; i++)
          {
            auto dview = xt::view(data, i);
            auto lview = xt::view(label, i);
            dview = ptr_to_dataloader->ptr_dataset->getitem(ptr_to_dataloader->item_indices[i]).getData();
            if(!lshapeCheck) lview = ptr_to_dataloader->ptr_dataset->getitem(ptr_to_dataloader->item_indices[i]).getLabel();
            else label = 0;
          }
          return Batch<DType, LType>(data, label);
        }
      }
      if (cursor == ptr_to_dataloader->nbatch - 1)
      {
        if (this->ptr_to_dataloader->drop_last)
        {
          dshape[0] = this->ptr_to_dataloader->batch_size;
          lshape[0] = this->ptr_to_dataloader->batch_size;
          data = xt::xarray<DType>::from_shape(dshape);
          label = xt::xarray<LType>::from_shape(lshape);
          for (int i = cursor * ptr_to_dataloader->batch_size; i < cursor * ptr_to_dataloader->batch_size + ptr_to_dataloader->batch_size; i++)
          {
            auto dview = xt::view(data, i - cursor * ptr_to_dataloader->batch_size);
            auto lview = xt::view(label, i - cursor * ptr_to_dataloader->batch_size);
            auto item = ptr_to_dataloader->ptr_dataset->getitem(ptr_to_dataloader->item_indices[i]);
            dview = item.getData();
            if(!lshapeCheck) lview = item.getLabel();
            else label = 0;
          }
          return Batch<DType, LType>(data, label);
        }
        else
        {
          dshape[0] = this->ptr_to_dataloader->batch_size + this->ptr_to_dataloader->last_remainder;
          lshape[0] = this->ptr_to_dataloader->batch_size + this->ptr_to_dataloader->last_remainder;
          data = xt::xarray<DType>::from_shape(dshape);
          label = xt::xarray<LType>::from_shape(lshape);
          for (int i = cursor * ptr_to_dataloader->batch_size; i < ptr_to_dataloader->ptr_dataset->len(); i++)
          {
            auto dview = xt::view(data, i - cursor * ptr_to_dataloader->batch_size);
            auto lview = xt::view(label, i - cursor * ptr_to_dataloader->batch_size);
            dview = ptr_to_dataloader->ptr_dataset->getitem(ptr_to_dataloader->item_indices[i]).getData();
            if(!lshapeCheck) lview = ptr_to_dataloader->ptr_dataset->getitem(ptr_to_dataloader->item_indices[i]).getLabel();
            else label = 0;
          }
          return Batch<DType, LType>(data, label);
        }
      }
      dshape[0] = this->ptr_to_dataloader->batch_size;
      lshape[0] = this->ptr_to_dataloader->batch_size;
      data = xt::xarray<DType>::from_shape(dshape);
      label = xt::xarray<LType>::from_shape(lshape);
      for (int i = cursor * ptr_to_dataloader->batch_size; i < cursor * ptr_to_dataloader->batch_size + ptr_to_dataloader->batch_size; i++)
      {
        auto dview = xt::view(data, i - cursor * ptr_to_dataloader->batch_size);
        auto lview = xt::view(label, i - cursor * ptr_to_dataloader->batch_size);
        dview = ptr_to_dataloader->ptr_dataset->getitem(ptr_to_dataloader->item_indices[i]).getData();
        if(!lshapeCheck) lview = ptr_to_dataloader->ptr_dataset->getitem(ptr_to_dataloader->item_indices[i]).getLabel();
        else label = 0;
      }
      return Batch<DType, LType>(data, label);
    }
  };
    // END of Iterator

    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};

#endif /* DATALOADER_H */
