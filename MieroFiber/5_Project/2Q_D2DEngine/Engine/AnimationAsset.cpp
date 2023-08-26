#include "pch.h"
#include "AnimationAsset.h"
#include "CommonApp.h"
#include <codecvt>
#include <fstream>
#include "nlohmann/json.hpp"

AnimationAsset::AnimationAsset()
	: m_pBitmap(nullptr)
{

}

AnimationAsset::~AnimationAsset()
{
	// 팩토리를 통해서 해제한다.
	CommonApp::m_pInstance->GetRenderer()->ReleaseD2DBitmapFromFile(m_pBitmap);
}

void AnimationAsset::Build()
{
	CommonApp::m_pInstance->GetRenderer()->CreateD2DBitmapFromFile(m_BitmapFilePath.c_str(), &m_pBitmap);
}

void AnimationAsset::SetBitmapFilePath(const WCHAR* szFilePath)
{
	m_BitmapFilePath = szFilePath;
}

void AnimationAsset::Save(const WCHAR* szFilePath) const
{
	nlohmann::ordered_json obj;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	obj["m_BitmapFilePath"] = converter.to_bytes(szFilePath);

	for (auto ani : m_Animations)
	{
		std::wstring animationName = ani.first;
		nlohmann::ordered_json animationObj;
		animationObj["Name"] = converter.to_bytes(animationName);

		int frameSize = static_cast<int>(ani.second.size());
		for (int i = 0; i < frameSize; i++)
		{
			framework::FRAME_INFO& frame = ani.second[i];
			nlohmann::ordered_json frameObj;
			frameObj["Source.left"] = frame.Source.left;
			frameObj["Source.top"] = frame.Source.top;
			frameObj["Source.right"] = frame.Source.right;
			frameObj["Source.bottom"] = frame.Source.bottom;
			frameObj["RenderTime"] = (frame.RenderTime);
			animationObj["m_Frames"].push_back(frameObj);
		}
		obj["m_Animations"].push_back(animationObj);
	}

	std::ofstream ofs(szFilePath);
	ofs << obj.dump(2);
	ofs.close();
}

bool AnimationAsset::Load(const WCHAR* szFilePath)
{
	std::ifstream ifs(szFilePath, std::ios::in);
	if (!ifs.is_open())
	{
		return false;
	}

	std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	nlohmann::ordered_json obj = nlohmann::ordered_json::parse(str);

	for (auto& animationObj : obj["m_Animations"])
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring animationName = converter.from_bytes(animationObj["Name"]);
		std::vector<framework::FRAME_INFO> frameVec;

		for (auto& frameObj : animationObj["m_Frames"])
		{
			framework::FRAME_INFO frame(
				{ frameObj["Source.left"], frameObj["Source.top"],frameObj["Source.right"], frameObj["Source.bottom"] },
				frameObj["RenderTime"]);
			frameVec.push_back(frame);
		}
		m_Animations.insert({ animationName, frameVec });
	}
	return true;
}