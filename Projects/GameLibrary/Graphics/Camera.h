#pragma once

namespace GameLibrary
{
	class CameraComponentDefinition;

	typedef Helium::StrongPtr<CameraComponentDefinition> CameraComponentDefinitionPtr;	
	typedef Helium::StrongPtr<const CameraComponentDefinition> ConstCameraComponentDefinition;


	//////////////////////////////////////////////////////////////////////////
	// CameraComponent
	class GAME_LIBRARY_API CameraComponent : public Helium::Component
	{
	public:
		HELIUM_DECLARE_COMPONENT( GameLibrary::CameraComponent, Helium::Component );
		static void PopulateMetaType( Helium::Reflect::MetaStruct& comp );

		CameraComponent();
		~CameraComponent();

		void Initialize( const CameraComponentDefinition &definition);

		Helium::Simd::Vector3 GetUp() const { return m_Up; }
		Helium::Name GetName() const { return m_Name; }
		float GetNearClip() const { return m_NearClip; }
		float GetFarClip() const { return m_FarClip; }
		float GetFov() const { return m_Fov; }

	private:
		Helium::Simd::Vector3 m_Up;
		Helium::Name m_Name;
		float m_NearClip;
		float m_FarClip;
		float m_Fov;
		bool m_Registered;
	};

	class GAME_LIBRARY_API CameraComponentDefinition : public Helium::ComponentDefinitionHelper<CameraComponent, CameraComponentDefinition>
	{
	public:
		HELIUM_DECLARE_CLASS( GameLibrary::CameraComponentDefinition, Helium::ComponentDefinition );
		static void PopulateMetaType( Helium::Reflect::MetaStruct& comp );

		CameraComponentDefinition();

		friend CameraComponent;

	private:
		Helium::Simd::Vector3 m_Up;
		Helium::Name m_Name;
		float m_NearClip;
		float m_FarClip;
		float m_Fov;
	};

	typedef Helium::ComponentPtr<CameraComponent> CameraComponentPtr;
}
