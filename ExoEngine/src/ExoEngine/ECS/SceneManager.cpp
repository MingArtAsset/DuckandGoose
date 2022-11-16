/*!*************************************************************************
****
\file SceneManager.cpp
\author Elton Teo Zhe Wei
\par DP email: e.teo@digipen.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM200
\date 2/11/2022
\brief  This file contains all the functions for loading and unloading the
level data into a JSON file. Currently on application end the ecs will be
saved into a json file for future loading

****************************************************************************
***/

#include "SceneManager.h"

namespace EM
{

	std::unique_ptr<SceneManager> SceneManager::m_instance;

	std::unique_ptr<SceneManager>& SceneManager::GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = std::make_unique<SceneManager>();
		}
		return m_instance;
	}

	//Initialize all registered components
	void SceneManager::Init()
	{
		p_ecs.RegisterComponent<Transform>();
		p_ecs.RegisterComponent<RigidBody>();
		p_ecs.RegisterComponent<Collider>();
		p_ecs.RegisterComponent<NameTag>();
		p_ecs.RegisterComponent<Sprite>();
		p_ecs.RegisterComponent<Logic>();
		p_ecs.RegisterComponent<Player>();
		p_ecs.RegisterComponent<Audio>();
	}

	//Due to restrictions of not using RTTR, components type cannot be deduced at runtime and hence needs to be checked manually
	//Will work on improving deserialization further
	bool SceneManager::Deserialize(const rapidjson::Value& obj)
	{
		p_ecs.ResetEntities();
		p_ecs.SetTotalEntitiesForWorldBuild(obj["Number of Entities"]["Entities"].GetUint());
		for (ComponentType i = 0; i < p_ecs.GetTotalRegisteredComponents(); ++i)
		{
			auto aliveTotal = p_ecs.GetTotalEntities();
			Entity aliveCount = 0;
			Entity j = 0;
			p_ecs.ClearArrayForWorldBuild(i);
			
			while(aliveCount < aliveTotal)
			{
				Signature signature(obj["EntitySignatures"][(j)].GetString());
				if (signature.any())
				{
					++aliveCount;
					if (signature.test(i))
					{
						//ADD COMPONENTS HERE FOR DESERIALIZE
						if (p_ecs.GetComponentTypeName(i) == "Transform")
						{
							Transform transform;
							if (transform.Deserialize(obj["Components"][p_ecs.GetComponentTypeName(i).c_str()][j].GetObj()))
							{
								p_ecs.AddComponent<Transform>(j, transform);
							}
						}
						if (p_ecs.GetComponentTypeName(i) == "RigidBody")
						{
							RigidBody rigidbody;
							if (rigidbody.Deserialize(obj["Components"][p_ecs.GetComponentTypeName(i).c_str()][j].GetObj()))
							{
								p_ecs.AddComponent<RigidBody>(j, rigidbody);
							}
						}
						if (p_ecs.GetComponentTypeName(i) == "Collider")
						{
							Collider collider;
							if (collider.Deserialize(obj["Components"][p_ecs.GetComponentTypeName(i).c_str()][j].GetObj()))
							{
								p_ecs.AddComponent<Collider>(j, collider);
							}
						}
						if (p_ecs.GetComponentTypeName(i) == "NameTag")
						{
							NameTag nametag;
							if (nametag.Deserialize(obj["Components"][p_ecs.GetComponentTypeName(i).c_str()][j].GetObj()))
							{
								p_ecs.AddComponent<NameTag>(j, nametag);
							}
						}
						if (p_ecs.GetComponentTypeName(i) == "Sprite")
						{
							Sprite sprite;
							if (sprite.Deserialize(obj["Components"][p_ecs.GetComponentTypeName(i).c_str()][j].GetObj()))
							{
								p_ecs.AddComponent<Sprite>(j, sprite);
							}
						}
						if (p_ecs.GetComponentTypeName(i) == "Logic")
						{
							Logic logic;
							if (logic.Deserialize(obj["Components"][p_ecs.GetComponentTypeName(i).c_str()][j].GetObj())) {}
							p_ecs.AddComponent<Logic>(j, logic);
						}
						if (p_ecs.GetComponentTypeName(i) == "Player")
						{
							Player player;
							if (player.Deserialize(obj["Components"][p_ecs.GetComponentTypeName(i).c_str()][j].GetObj()))
							{
								p_ecs.AddComponent<Player>(j, player);
							}
						}
						if (p_ecs.GetComponentTypeName(i) == "Audio")
						{
							Audio mAudio;
							if (mAudio.Deserialize(obj["Components"][p_ecs.GetComponentTypeName(i).c_str()][j].GetObj()))
							{
								p_ecs.AddComponent<Audio>(j, mAudio);
							}
						}

					}
				}
				++j;
			}
			for (Entity j = 0; j < MAX_ENTITIES; ++j)
			{
				p_ecs.GetEntityToIndexMapECS(i)[j] = obj["EntityToIndexMap"][p_ecs.GetComponentTypeName(i).c_str()][j].GetUint();
				p_ecs.GetIndexToEntityMapECS(i)[j] = obj["IndexToEntityMap"][p_ecs.GetComponentTypeName(i).c_str()][j].GetUint();
			}
		}
		return true;
	}


	//Due to restrictions of not using RTTR, components type cannot be deduced at runtime and hence needs to be checked manually
	//Will work on improving serialization further
	bool SceneManager::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->StartObject();

		writer->Key("Number of Entities");
		writer->StartObject();
		writer->Key("Entities");
		writer->Uint(p_ecs.GetTotalEntities());
		writer->EndObject();

		writer->Key("EntityToIndexMap");
		writer->StartObject();
		for (ComponentType i = 0; i < p_ecs.GetTotalRegisteredComponents(); ++i)
		{
			writer->Key(p_ecs.GetComponentTypeName(i).c_str());
			writer->StartArray();
			for (auto j = p_ecs.GetEntityToIndexMapECS(i).begin(); j != p_ecs.GetEntityToIndexMapECS(i).end(); ++j)
			{
				writer->Uint64(*j);
			}
			writer->EndArray();
		}
		writer->EndObject();

		writer->Key("IndexToEntityMap");
		writer->StartObject();
		for (ComponentType i = 0; i < p_ecs.GetTotalRegisteredComponents(); ++i)
		{
			writer->Key(p_ecs.GetComponentTypeName(i).c_str());
			writer->StartArray();
			for (auto j = p_ecs.GetIndexToEntityMapECS(i).begin(); j != p_ecs.GetIndexToEntityMapECS(i).end(); ++j)
			{
				writer->Uint(*j);
			}
			writer->EndArray();
		}
		writer->EndObject();

		writer->Key("EntitySignatures");
		writer->StartArray();
		auto aliveTotal = p_ecs.GetTotalEntities();
		Entity aliveCount = 0;
		Entity iterEntity = 0;
		while (aliveCount < aliveTotal)
		{
			writer->String(p_ecs.GetEntitySignature(iterEntity).to_string().c_str());
			if (p_ecs.GetEntitySignature(iterEntity).any())
				aliveCount++;
			iterEntity++;
		}
		writer->EndArray();

		writer->Key("Components");
		writer->StartObject();
		for (ComponentType i = 0; i < p_ecs.GetTotalRegisteredComponents(); ++i)
		{
			writer->Key(p_ecs.GetComponentTypeName(i).c_str());
			writer->StartArray();
			auto aliveTotal = p_ecs.GetTotalEntities();
			Entity aliveCount = 0;
			Entity j = 0;
			while (aliveCount < aliveTotal)
			{
				if(p_ecs.GetEntitySignature(j).any())
					aliveCount++;
				if (p_ecs.GetEntitySignature(j).test(i))
				{
					//ADD COMPONENTS HERE FOR SERIALIZE
					if (p_ecs.GetComponentTypeName(i) == "Transform")
					{
						p_ecs.GetComponent<Transform>(j).Serialize(writer);
					}
					if (p_ecs.GetComponentTypeName(i) == "RigidBody")
					{
						p_ecs.GetComponent<RigidBody>(j).Serialize(writer);
					}
					if (p_ecs.GetComponentTypeName(i) == "Collider")
					{
						p_ecs.GetComponent<Collider>(j).Serialize(writer);
					}
					if (p_ecs.GetComponentTypeName(i) == "NameTag")
					{
						p_ecs.GetComponent<NameTag>(j).Serialize(writer);
					}
					if (p_ecs.GetComponentTypeName(i) == "Sprite")
					{
						p_ecs.GetComponent<Sprite>(j).Serialize(writer);
					}
					if (p_ecs.GetComponentTypeName(i) == "Logic")
					{
						p_ecs.GetComponent<Logic>(j).Serialize(writer);
					}
					if (p_ecs.GetComponentTypeName(i) == "Player")
					{
						p_ecs.GetComponent<Player>(j).Serialize(writer);
					}
					if (p_ecs.GetComponentTypeName(i) == "Audio")
					{
						p_ecs.GetComponent<Audio>(j).Serialize(writer);
					}
				}
				j++;
			}
			writer->EndArray();
		}
		writer->EndObject();

		writer->EndObject();
		return true;
	}
}