#pragma once

#include <functional>
#include <array>

template<unsigned int Dimension> class Vector
{
public:
    template<typename... floats> Vector(floats... f)
        : parameters{ {(static_cast<float>(f))...} }
    {
        static_assert(Dimension > 0, "Vector dimension cannot be zero");
        static_assert(sizeof...(floats) == Dimension, "Vector dimension does not match arguments given");
    }

    float& operator[](unsigned int i) { return parameters.at(i); }
    float operator[](unsigned int i) const { return parameters.at(i); }
    inline Vector<Dimension> operator+(const Vector<Dimension>& other) const { return Operate(other, std::plus<float>()); }
    inline Vector<Dimension> operator-(const Vector<Dimension>& other) const { return Operate(other, std::minus<float>()); }
    inline Vector<Dimension> operator/(float scalar) const { return OperateScalar(scalar, std::divides<float>()); }
    inline Vector<Dimension> operator*(float scalar) const { return OperateScalar(scalar, std::multiplies<float>()); }

    float SqrMagnitude() const
    {
        float mag = 0;
        for (int i = 0; i < Dimension; i++)
        {
            mag += (parameters[i] * parameters[i]);
        }
        return mag;
    }

    float Magnitude() const
    {
        return sqrt(SqrMagnitude());
    }

    Vector<Dimension> Normalized() const
    {
        float magnitude = Magnitude();
        if (magnitude == 0.0f)
        {
            return Zero();
        }
        float inverse = 1 / magnitude;
        return (*this) * inverse;
    }

    float Dot(const Vector<Dimension>& other) const
    {
        float product = 0;
        for (int i = 0; i < Dimension; i++)
        {
            product += (parameters[i] * other.parameters[i]);
        }
        return product;
    }

    /// <summary>
    /// Translates a Dot Product of non-normal vectors to the same
    /// value you would get doing a Dot Product of normal vectors,
    /// but without needing to normalize them first.
    /// </summary>
    /// <param name="other"></param>
    /// <returns></returns>
    float UnNormalDot(const Vector<Dimension>& other) const
    {
        return Dot(other) / sqrt((SqrMagnitude() * other.SqrMagnitude()));
    }

    float SqrDistance(const Vector<Dimension>& other) const
    {
        return ((*this) - other).SqrMagnitude();
    }

    float Distance(const Vector<Dimension>& other) const
    {
        return ((*this) - other).Magnitude();
    }

    static Vector<Dimension> Zero() { return Vector<Dimension>(); }

    void Print() const
    {
        std::cout << "(";
        for (int i = 0; i < Dimension; i++)
        {
            std::cout << parameters[i];
            if (i != Dimension - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << ")\n";
    }

protected:
    std::array<float, Dimension> parameters;

    Vector() : parameters{ 0 } { }

private:
    Vector<Dimension> Operate(const Vector<Dimension>& other, std::function<float(float, float)> operation) const
    {
        auto result = Vector<Dimension>();
        for (int i = 0; i < Dimension; i++)
        {
            result[i] = operation(parameters[i], other.parameters[i]);
        }

        return result;
    }

    Vector<Dimension> OperateScalar(float scalar, std::function<float(float, float)> operation) const
    {
        auto result = Vector<Dimension>();
        for (int i = 0; i < Dimension; i++)
        {
            result[i] = operation(parameters[i], scalar);
        }

        return result;
    }
};

class Vector2 : public Vector<2>
{
public:
    Vector2(float x, float y) : Vector<2>(x, y) { }
    Vector2(Vector<2>&& base) : Vector<2>(std::move(base)) { }

    float& X = parameters[0];
    float& Y = parameters[1];
};

class Vector3 : public Vector<3>
{
public:
    Vector3(float x, float y, float z) : Vector<3>(x, y, z) { }
    Vector3(Vector<3>&& base) : Vector<3>(std::move(base)) { }

    float& X = parameters[0];
    float& Y = parameters[1];
    float& Z = parameters[2];

    Vector3 Cross(const Vector3& other) const
    {
        auto result = Vector3();
        for (int i = 0; i < 3; i++)
        {
            result[i] = (parameters[(i + 1) % 3] * other.parameters[(i + 2) % 3]) -
                (parameters[(i + 2) % 3] * other.parameters[(i + 1) % 3]);
        }
        return result;
    }

protected:
    Vector3() : Vector<3>() { }
};
