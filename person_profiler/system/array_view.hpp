#pragma once

#include <vector>
#include <type_traits>
#include <assert.h>
#include <stdint.h>


        template <typename T>
        struct array_view_iterator {
            typedef ptrdiff_t difference_type;
            typedef std::forward_iterator_tag iterator_category;
            typedef T* pointer;
            typedef T& reference;
            typedef T value_type;

            array_view_iterator(T* ptr)
                : ptr_(ptr) {}

            bool operator!=(array_view_iterator const& other) const { return ptr_ != other.ptr_; }
            bool operator==(array_view_iterator const& other) const { return ptr_ == other.ptr_; }

            array_view_iterator& operator++() {
                ++ptr_;
                return *this;
            }

            array_view_iterator operator++(int) {
                ++ptr_;
                return array_view_iterator(ptr_ - 1);
            }

            T& operator*() const { return *ptr_; }

        private:
            T * ptr_;
        };

        // TODO: test it
        template <typename T>
        struct array_view_reverse_iterator {
            typedef ptrdiff_t difference_type;
            typedef std::forward_iterator_tag iterator_category;
            typedef T* pointer;
            typedef T& reference;
            typedef T value_type;

            array_view_reverse_iterator(T* ptr)
                : ptr_(ptr) {}

            bool operator!=(array_view_reverse_iterator const& other) const { return ptr_ != other.ptr_; }
            bool operator==(array_view_reverse_iterator const& other) const { return ptr_ == other.ptr_; }

            array_view_iterator<T>& operator++() {
                --ptr_;
                return *this;
            }

            array_view_iterator<T> operator++(int) {
                --ptr_;
                return array_view_reverse_iterator(ptr_ + 1);
            }

            T& operator*() const { return *ptr_; }

        private:
            T * ptr_;
        };


        template <typename T>
        struct array_view {

            using value_type = std::add_const_t<std::decay_t<T>>;
            using pointer_type = value_type * ;
            using reference_type = value_type & ;

            template <unsigned N>
            array_view(value_type(&v)[N])
                : ptr_(&v)
                , size_(N) {}

            array_view(pointer_type ptr, size_t size)
                : ptr_(ptr)
                , size_(size) {}

            array_view(std::vector<T> const& vec)
                : ptr_(vec.data())
                , size_(vec.size()) {}

            array_view(array_view const& other) {
                ptr_ = other.ptr_;
                size_ = other.size_;
            }

            array_view(T const& value)
                : ptr_(&value),
                size_(1)
            {}

            array_view()
                : ptr_(nullptr)
                , size_(0) {}

            ~array_view() {}

            bool empty() const {
                return size_ == 0 || ptr_ == nullptr;
            }

            size_t size() const { return size_; }

            reference_type operator[](size_t index) const {
                assert(index < size_);
                return *(ptr_ + index);
            }

            reference_type front() const {
                assert(ptr_ != nullptr && size_ > 0);
                return *ptr_;
            }

            reference_type back() const {
                assert(ptr_ != nullptr && size_ > 0);
                return *(ptr_ + size_ - 1);
            }

            array_view_iterator<value_type> begin() const { return array_view_iterator<value_type>(ptr_); }
            array_view_iterator<value_type> end() const { return array_view_iterator<value_type>(ptr_ + size_); }

            array_view_reverse_iterator<value_type> rbegin() const {
                return array_view_iterator<value_type>(ptr_ + size_ - 1);
            }
            array_view_reverse_iterator<value_type> rend() const {
                return array_view_iterator<value_type>(ptr_ - 1);
            }

        private:
            pointer_type ptr_;
            size_t size_;
        };