
#pragma once
/*
 属性装饰器
 绑定属性值，设置访问过滤：get，set
*/
template<class T>
class Property {
public:
    typedef T(*Getter)(const T&);
    typedef void (*Setter)(T&);

#pragma region 构造函数
    /*
     绑定属性值，设置get方法和set方法
     如果不提供，则不对访问进行过滤
    */
    Property(T& value, Getter getter = nullptr, Setter setter = nullptr)
        : _value(value), _getter(getter), _setter(setter) {}
    /*
     绑定属性值，设置get方法和set方法
     如果不提供，则不对访问进行过滤
    */
    Property(T& value, Setter setter)
        : _value(value), _getter(nullptr), _setter(setter) {}
#pragma endregion
    
    /*
     属性的Get方法
    */
    T Get() const {
        if (_getter)
            return _getter(_value);
        else
            return _value;
    }
    /*
     属性的Set方法
    */
    void Set(T value) {
        if (_setter)
            _setter(_value);
        else
            _value = value;
    }

private:
    T& _value;          // 绑定的属性值
    Getter _getter;     // get的方法
    Setter _setter;     // set的方法
};

/*
 只读属性装饰器
 绑定属性值，设置访问过滤：get
*/
template<class T>
class ROProperty {

public:
    typedef T(*Getter)(const T&);
    typedef void (*Setter)(T&);
    /*
     绑定属性值，设置get方法
     如果不提供，则不对访问进行过滤
    */
    ROProperty(T& value, Getter getter = nullptr)
        : _value(value), _getter(getter) {}
    /*
     属性的Get方法
    */
    T Get() const {
        if (_getter)
            return _getter(_value);
        else
            return _value;
    }
private:
    T& _value;          // 绑定的属性值
    Getter _getter;     // get的方法
};
