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

	//���ʶ�Ӧ����
	FString Name;

	//�������������д��λ��
	FString MaterialPath;

	//��ɫ��ͼ�������д��λ��
	FString BaseColorPath;

	//������ͼ�������д��λ��
	FString NormalPath;

	//ORM��ͼ�������д��λ��
	FString OcclusionRoughnessMetallicPath;



};