#pragma once

class UAlexMaterial
{
public:
	UAlexMaterial()
	{
	};
	~UAlexMaterial()
	{
	};

	//材质对应名字
	FString Name;

	//材质球在引擎中存放位置
	FString MaterialPath;

	//颜色贴图在引擎中存放位置
	FString BaseColorPath;

	//法线贴图在引擎中存放位置
	FString NormalPath;

	//ORM贴图在引擎中存放位置
	FString OcclusionRoughnessMetallicPath;



};