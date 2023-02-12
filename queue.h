#ifndef DATABASE_QUEUE_H
#define DATABASE_QUEUE_H
#pragma once

#include <iostream>

template<typename Type>
class Queue{
    struct node{
        Type cell;
        node* next;
    };


    class QueueIterator{

        friend class Queue;

    private:
        node* current;
        node* end;

        QueueIterator(node* q){
            current = q;
            end = nullptr;
        }

    public:

        const Type& operator*(){
            if (this != nullptr){
                return this->current->cell;
            } else{
                throw std::out_of_range("Out of range\n");
            }

        }

        QueueIterator& operator++(){
            if (this->current != this->end){
                this->current = this->current->next;
                return *this;
            }else{
                this->current = this->end;
                return *this;
            }
        }

        QueueIterator& operator++(int){
            if (this->current!= this->end){
//                QueueIterator& tmp = this;
                auto tmp = this;
                this->current = this->current->next;
                return *tmp;
            }
            return *this;

        }

        bool operator==(QueueIterator iter2){
            if (this->current == iter2.current){
                return true;
            } else{
                return false;
            }
        }

        bool operator!=(QueueIterator iter2){
            if (this->current == iter2.current){
                return false;
            } else{
                return true;
            }
        }

    };

private:

    node* head;
    node* tail;

    node* cast(const Type& cell){
        node* tmp;

        try {
            tmp = new node;
        } catch (std::bad_alloc &ba) {
            throw (ba);
        }

        tmp->cell = cell;
        tmp->next = nullptr;
        return tmp;
    }


public:
    Queue(){

        head = nullptr;
        tail = nullptr;
    }

    //copy
    Queue(const Queue& old){

        this->head = nullptr;
        this->tail = nullptr;

        node* tmp = old.head;
        while (tmp){
            this->push(tmp->cell);
            tmp = tmp->next;
        }
    }

    //move
    Queue(Queue&& old) {

        this->head = old.head;
        this->tail = old.tail;

        old.head = nullptr;
        old.tail = nullptr;
    }

    Queue(Type& small){
        head = nullptr;
        tail = nullptr;
        this->push(small);
    }

    const Type& front() const{
        return head->cell;
    }
    const Type& back() const{
        return tail->cell;
    }

    template<typename R>
    Queue<R> selector(R f(const Type&)) const{
        auto modified = Queue<R>();
        QueueIterator it = this->begin();
        while (it != this->end()){
            modified.push(f(*it));
            it++;
        }
        return modified;
    }

    QueueIterator begin(){
        return QueueIterator(this->head);

    }
    QueueIterator end(){
        return QueueIterator(nullptr);
    }

    const QueueIterator begin() const{
        return QueueIterator(this->head);

    }
    const QueueIterator end() const{
        return QueueIterator(nullptr);
    }


    void push(const Type& cell){

        auto new_cell = cast(cell);

        if (tail == nullptr){
            tail = new_cell;
            head = new_cell;
            return;
        }

        tail->next = new_cell;
        tail = new_cell;

    }

    void pop(){
        if (head == nullptr){
            throw (std::out_of_range ("No elements in queue\n"));
        }

        auto tmp = head;
        head = tmp->next;
        delete tmp;
    }

    bool empty(){
        if (head != nullptr){
            return false;
        }else{
            return true;
        }
    }

    bool empty() const{
        if (head != nullptr){
            return false;
        }else{
            return true;
        }
    }
//
//    table::Info operator += (const table::Info& t2);
    Queue& operator=(const Queue &q2){
        if (&q2 == this){
            return *this;
        }

        while (!this->empty()){
            this->pop();
        }
        node* tmp = q2.head;
        while (tmp){
            this->push(tmp->cell);
            tmp = tmp->next;
        }

        return *this;

    }

    Queue& operator=(Queue &&q2){

        if (&q2 == this) {
            return *this;
        }
        while (!this->empty()){
            this->pop();
        }
        this->head = q2.head;
        this->tail = q2.tail;

        q2.head = nullptr;
        q2.tail = nullptr;

        return *this;

    }
};

#endif //DATABASE_QUEUE_H
