#pragma once
#include "../dxlib_ext/dxlib_ext.h"

//２年制ゲームプログラマー　渡辺さんから
//
//

namespace wta
{
    // tnl::Vector3をVECTORに変換する
    // args ... tnl::Vector3
    // return ... VECTOR
    // ex ...     VECTOR vec = wta::ConvertToVECTOR(tl_vec);
    inline VECTOR ConvertToVECTOR(const tnl::Vector3& tnl_pos)
    {
        return VGet(tnl_pos.x, tnl_pos.y, tnl_pos.z);
    }

    // VECTORをtnl::Vector3に変換する
    // args ... VECTOR
    // return ... tnl::Vector3
    // ex ...     tnl::Vector3 tnl_vec = wta::ConvertToVECTOR(vec);
    inline tnl::Vector3 ConvertToTnl(const VECTOR& vec_pos)
    {
        return tnl::Vector3(vec_pos.x, vec_pos.y, vec_pos.z);
    }

    // tnl::QuaternionからDXlibのMATRIXへの変換
    // args ... tnl::Quaternion
    // ex ...     MATRIX matrix = wta::ConvertToVECTOR(tnl_quaternion);
    inline MATRIX ConvertQuaternionToMatrix(const tnl::Quaternion& quaternion)
    {
        // tnl::Quaternionをtnl::Matrixに変換
        tnl::Matrix mat = quaternion.getMatrix();
        // MATRIX型定義
        MATRIX dxm;
        // MATRIX型のサイズで、tnl::MatrixをMATRIXに変換
        memcpy(&dxm, &mat.m, sizeof(float) * 16);

        return dxm;
    }
}