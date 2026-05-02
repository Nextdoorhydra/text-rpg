#pragma once
#include <iostream>
#include <algorithm> // std::sort 사용
#include <utility>

template<typename T>
class Inventory {
public:
    using Slot = std::pair<T, int>;

private:
    Slot* pItems_;  // Slot들의 동적 배열
    int capacity_;  // 최대 용량
    int size_;      // 현재 아이템 종류(슬롯) 개수

public:
    // 이터레이터 지원
    using iterator = Slot*;
    using const_iterator = const Slot*;

    iterator begin() { return pItems_; }
    iterator end() { return pItems_ + size_; }

    const_iterator begin() const { return pItems_; }
    const_iterator end() const { return pItems_ + size_; }

    Inventory(int capacity = 10) : capacity_(capacity), size_(0) {
        pItems_ = new Slot[capacity_];
    }

    ~Inventory() {
        delete[] pItems_;
    }

    // 복사 생성자
    Inventory(const Inventory& other) : capacity_(other.capacity_), size_(other.size_) {
        pItems_ = new Slot[capacity_];
        for (int i = 0; i < size_; ++i) pItems_[i] = other.pItems_[i];
    }

    // 복사 대입 생성자
    Inventory& operator=(const Inventory& other) {
        if (this != &other) {
            delete[] pItems_;
            capacity_ = other.capacity_;
            size_ = other.size_;
            pItems_ = new Slot[capacity_];
            for (int i = 0; i < size_; ++i) pItems_[i] = other.pItems_[i];
        }
        return *this;
    }

    void Resize(int newCapacity) {
        if (newCapacity <= capacity_) return;

        Slot* newArray = new Slot[newCapacity];
        for (int i = 0; i < size_; ++i) {
            newArray[i] = pItems_[i];
        }
        delete[] pItems_;

        pItems_ = newArray;
        capacity_ = newCapacity;
    }

    // 아이템 추가 (중복 확인 및 카운트 증가 로직 포함)
    int AddItem(const T& item, int amount = 1) {
        // 이미 인벤토리에 존재하는 아이템인지 검사
        for (int i = 0; i < size_; ++i) {
            if (pItems_[i].first == item) {
                pItems_[i].second += amount;
                return i; // 위치 반환
            }
        }

        // 존재하지 않는 새로운 아이템이라면 슬롯 추가
        if (size_ >= capacity_) {
            int newCapacity = capacity_ * 2;
            Resize(newCapacity);
        }

        pItems_[size_] = std::make_pair(item, amount);
        size_++;
        return size_ - 1;
    }

    bool TryUseItem(const T& item) {
        for (int i = 0; i < size_; ++i) {
            if (pItems_[i].first == item) { // 아이템을 찾음
                if (pItems_[i].second > 0) {
                    pItems_[i].second--; // 수량 감소

                    return true;
                }
            }
        }
        return false;
    }

    int GetItemCount(const T& item) const {
        for (int i = 0; i < size_; ++i) {
            if (pItems_[i].first == item) {
                return pItems_[i].second;
            }
        }
        return 0; // 인벤토리에 없으면 0개 반환
    }

    Slot& operator[](int index) {
        return pItems_[index];
    }

    const Slot& operator[](int index) const {
        return pItems_[index];
    }

    template <typename CompareFunction>
    void SortItems(CompareFunction comp) {
        std::sort(pItems_, pItems_ + size_, comp);
    }

    int GetSize() const { return size_; }
    int GetCapacity() const { return capacity_; }
};