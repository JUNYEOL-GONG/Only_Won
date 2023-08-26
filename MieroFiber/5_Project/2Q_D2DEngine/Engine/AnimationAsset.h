#pragma once

#include "ReferenceCounter.h"

#include <unordered_map>
#include <vector>

/// <summary>
/// �ִϸ��̼ǿ� �ʿ��� ������
/// �������� �ִϸ��̼� ������ ��� �ν��Ͻ� ���� ���� ���۷��� ī���͸����� �����Ѵ�.
/// </summary>
class AnimationAsset :
    public ReferenceCounter
{
// �ı��� Release�� , ������ D2DRenderer�� �ϰ��Ѵ�.
public:
	AnimationAsset();
	virtual ~AnimationAsset();

public:
	ID2D1Bitmap* m_pBitmap;
	std::wstring m_BitmapFilePath;

	std::unordered_map<std::wstring, std::vector<framework::FRAME_INFO>> m_Animations = {};	// �ִϸ��̼� �̸�, ���������� ��

	void SetBitmapFilePath(const WCHAR* szFilePath);
	void Build();

	void Save(const WCHAR* szFilePath) const;
	bool Load(const WCHAR* szFilePath);
};

