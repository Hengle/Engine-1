//----------------------------------------------------------------------------------------------------------------------
// AaBox.h
//
// Copyright (C) 2010 WhiteMoon Dreams, Inc.
// All Rights Reserved
//----------------------------------------------------------------------------------------------------------------------

#pragma once
#ifndef LUNAR_CORE_AA_BOX_H
#define LUNAR_CORE_AA_BOX_H

#include "Core/Vector3.h"

namespace Lunar
{
    class Matrix44;

    /// Axis-aligned bounding box.
    L_ALIGN_PRE( 16 ) class LUNAR_CORE_API AaBox
    {
    public:
        /// @name Construction/Destruction
        //@{
        AaBox();
        AaBox( const Vector3& rMinimum, const Vector3& rMaximum );
        //@}

        /// @name Data Access
        //@{
        void Set( const Vector3& rMinimum, const Vector3& rMaximum );
        inline const Vector3& GetMinimum() const;
        inline const Vector3& GetMaximum() const;

        void Expand( const Vector3& rPoint );

        void TransformBy( const Matrix44& rTransform );
        //@}

    private:
        /// Box minimum.
        Vector3 m_minimum;
        /// Box maximum.
        Vector3 m_maximum;
    } L_ALIGN_POST( 16 );
}

#include "Core/AaBox.inl"

#endif  // LUNAR_CORE_AA_BOX_H