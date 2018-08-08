#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

class noncopyable
{
protected:
    noncopyable(){};
    ~noncopyable(){};

    noncopyable(const noncopyable&)= delete;
    const noncopyable& operator=(const noncopyable&)= delete;
};

#endif