#pragma once
#include "../dxlib_ext/dxlib_ext.h"

//�Q�N���Q�[���v���O���}�[�@�n�ӂ��񂩂�
//
//

namespace wta
{
    // tnl::Vector3��VECTOR�ɕϊ�����
    // args ... tnl::Vector3
    // return ... VECTOR
    // ex ...     VECTOR vec = wta::ConvertToVECTOR(tl_vec);
    inline VECTOR ConvertToVECTOR(const tnl::Vector3& tnl_pos)
    {
        return VGet(tnl_pos.x, tnl_pos.y, tnl_pos.z);
    }

    // VECTOR��tnl::Vector3�ɕϊ�����
    // args ... VECTOR
    // return ... tnl::Vector3
    // ex ...     tnl::Vector3 tnl_vec = wta::ConvertToVECTOR(vec);
    inline tnl::Vector3 ConvertToTnl(const VECTOR& vec_pos)
    {
        return tnl::Vector3(vec_pos.x, vec_pos.y, vec_pos.z);
    }

    // tnl::Quaternion����DXlib��MATRIX�ւ̕ϊ�
    // args ... tnl::Quaternion
    // ex ...     MATRIX matrix = wta::ConvertToVECTOR(tnl_quaternion);
    inline MATRIX ConvertQuaternionToMatrix(const tnl::Quaternion& quaternion)
    {
        // tnl::Quaternion��tnl::Matrix�ɕϊ�
        tnl::Matrix mat = quaternion.getMatrix();
        // MATRIX�^��`
        MATRIX dxm;
        // MATRIX�^�̃T�C�Y�ŁAtnl::Matrix��MATRIX�ɕϊ�
        memcpy(&dxm, &mat.m, sizeof(float) * 16);

        return dxm;
    }
}