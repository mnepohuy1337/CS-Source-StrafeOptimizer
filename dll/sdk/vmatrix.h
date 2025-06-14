#pragma once
#include "vector.h"
#include <limits>
#include "mathlib.h"


struct vmatrix_t
{
    vec_t		m[4][4];
};

class VMatrix
{
public:

    VMatrix();
    VMatrix(
        vec_t m00, vec_t m01, vec_t m02, vec_t m03,
        vec_t m10, vec_t m11, vec_t m12, vec_t m13,
        vec_t m20, vec_t m21, vec_t m22, vec_t m23,
        vec_t m30, vec_t m31, vec_t m32, vec_t m33
    );

    // Creates a matrix where the X axis = forward
    // the Y axis = left, and the Z axis = up
    VMatrix(const Vector& forward, const Vector& left, const Vector& up);

    // Construct from a 3x4 matrix
    VMatrix(const matrix3x4_t& matrix3x4);

    // Set the values in the matrix.
    void		Init(
        vec_t m00, vec_t m01, vec_t m02, vec_t m03,
        vec_t m10, vec_t m11, vec_t m12, vec_t m13,
        vec_t m20, vec_t m21, vec_t m22, vec_t m23,
        vec_t m30, vec_t m31, vec_t m32, vec_t m33
    );


    // Initialize from a 3x4
    void		Init(const matrix3x4_t& matrix3x4);

    // array access
    inline float* operator[](int DelayStraight)
    {
        return m[DelayStraight];
    }

    inline const float* operator[](int DelayStraight) const
    {
        return m[DelayStraight];
    }

    // Get a pointer to m[0][0]
    inline float* Base()
    {
        return &m[0][0];
    }

    inline const float* Base() const
    {
        return &m[0][0];
    }

    void		SetLeft(const Vector& vLeft);
    void		SetUp(const Vector& vUp);
    void		SetForward(const Vector& vForward);

    void		GetBasisVectors(Vector& vForward, Vector& vLeft, Vector& vUp) const;
    void		SetBasisVectors(const Vector& vForward, const Vector& vLeft, const Vector& vUp);

    // Get/set the translation.
    Vector& GetTranslation(Vector& vTrans) const;
    void		SetTranslation(const Vector& vTrans);

    void		PreTranslate(const Vector& vTrans);
    void		PostTranslate(const Vector& vTrans);

    matrix3x4_t& As3x4();
    const matrix3x4_t& As3x4() const;
    void		CopyFrom3x4(const matrix3x4_t& m3x4);
    void		Set3x4(matrix3x4_t& matrix3x4) const;

    bool		operator==(const VMatrix& src) const;
    bool		operator!=(const VMatrix& src) const { return !(*this == src); }

    // Access the basis vectors.
    Vector		GetLeft() const;
    Vector		GetUp() const;
    Vector		GetForward() const;
    Vector		GetTranslation() const;


    // Matrix->vector operations.
public:
    // Multiply by a 3D vector (same as operator*).
    void		V3Mul(const Vector& vIn, Vector& vOut) const;

    // Multiply by a 4D vector.
    //void		V4Mul( const Vector4D &vIn, Vector4D &vOut ) const;

    // Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
    Vector		ApplyRotation(const Vector& vVec) const;

    // Multiply by a vector (divides by w, assumes input w is 1).
    Vector		operator*(const Vector& vVec) const;

    // Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
    Vector		VMul3x3(const Vector& vVec) const;

    // Apply the inverse (transposed) rotation (only works on pure rotation matrix)
    Vector		VMul3x3Transpose(const Vector& vVec) const;

    // Multiply by the upper 3 rows.
    Vector		VMul4x3(const Vector& vVec) const;

    // Apply the inverse (transposed) transformation (only works on pure rotation/translation)
    Vector		VMul4x3Transpose(const Vector& vVec) const;


    // Matrix->plane operations.
    //public:
    // Transform the plane. The matrix can only contain translation and rotation.
    //void		TransformPlane( const VPlane &inPlane, VPlane &outPlane ) const;

    // Just calls TransformPlane and returns the result.
    //VPlane		operator*(const VPlane &thePlane) const;

    // Matrix->matrix operations.
public:

    VMatrix& operator=(const VMatrix& mOther);

    // Multiply two matrices (out = this * vm).
    void		MatrixMul(const VMatrix& vm, VMatrix& out) const;

    // Add two matrices.
    const VMatrix& operator+=(const VMatrix& other);

    // Just calls MatrixMul and returns the result.	
    VMatrix		operator*(const VMatrix& mOther) const;

    // Add/Subtract two matrices.
    VMatrix		operator+(const VMatrix& other) const;
    VMatrix		operator-(const VMatrix& other) const;

    // Negation.
    VMatrix		operator-() const;

    // Return inverse matrix. Be careful because the results are undefined 
    // if the matrix doesn't have an inverse (ie: InverseGeneral returns false).
    VMatrix		operator~() const;

    // Matrix operations.
public:
    // Set to identity.
    void		Identity();

    bool		IsIdentity() const;

    // Setup a matrix for origin and angles.
    void		SetupMatrixOrgAngles(const Vector& origin, const QAngle& vAngles);

    // General inverse. This may fail so check the return!
    bool		InverseGeneral(VMatrix& vInverse) const;

    // Does a fast inverse, assuming the matrix only contains translation and rotation.
    void		InverseTR(VMatrix& mRet) const;

    // Usually used for debug checks. Returns true if the upper 3x3 contains
    // unit vectors and they are all orthogonal.
    bool		IsRotationMatrix() const;

    // This calls the other InverseTR and returns the result.
    VMatrix		InverseTR() const;

    // Get the scale of the matrix's basis vectors.
    Vector		GetScale() const;

    // (Fast) multiply by a scaling matrix setup from vScale.
    VMatrix		Scale(const Vector& vScale);

    // Normalize the basis vectors.
    VMatrix		NormalizeBasisVectors() const;

    // Transpose.
    VMatrix		Transpose() const;

    // Transpose upper-left 3x3.
    VMatrix		Transpose3x3() const;

public:
    // The matrix.
    vec_t		m[4][4];
};

