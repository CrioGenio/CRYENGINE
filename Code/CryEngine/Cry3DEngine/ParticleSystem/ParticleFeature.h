// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

// -------------------------------------------------------------------------
//  Created:     24/09/2014 by Filipe amim
//  Description:
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ParticleUpdate.h"
#include "Features/ParamTraits.h"

#include <CrySerialization/IArchive.h>
#include <CrySerialization/Color.h>
#include <CrySerialization/ClassFactory.h>

#include <CryRenderer/IGpuParticles.h>
#include <CryCore/Dispatcher.h>

namespace pfx2
{

class CParticleComponentRuntime;

class CParticleFeature : public IParticleFeature
{
public:
	static bool RegisterFeature(const SParticleFeatureParams& params);
	~CParticleFeature() {}

	// IParticleFeature
	void                        SetEnabled(bool enabled) override                 { m_enabled.Set(enabled); }
	bool                        IsEnabled() const override                        { return m_enabled; }
	void                        Serialize(Serialization::IArchive& ar) override;
	uint                        GetNumConnectors() const override                 { return 0; }
	const char*                 GetConnectorName(uint connectorId) const override { return nullptr; }
	void                        ConnectTo(const char* pOtherName) override        {}
	void                        DisconnectFrom(const char* pOtherName) override   {}
	uint                        GetNumResources() const override                  { return 0; }
	const char*                 GetResourceName(uint resourceId) const override   { return nullptr; }
	// ~IParticleFeature

	// Initialization
	virtual bool              ResolveDependency(CParticleComponent* pComponent)                         { return true; }
	virtual void              AddToComponent(CParticleComponent* pComponent, SComponentParams* pParams) {}
	virtual EFeatureType      GetFeatureType()                                                          { return EFT_Generic; }
	virtual bool              CanMakeRuntime(CParticleEmitter* pEmitter) const                          { return true; }

	// Runtime and instance initialization
	virtual void MainPreUpdate(CParticleComponentRuntime* pComponentRuntime) {}

	virtual void InitSubInstances(const SUpdateContext& context, SUpdateRange instanceRange) {}

	virtual void GetSpatialExtents(const SUpdateContext& context, TConstArray<float> scales, TVarArray<float> extents) {}

	virtual void GetEmitOffset(const SUpdateContext& context, TParticleId parentId, Vec3& offset) {}

	// Particle initialization
	virtual void SpawnParticles(const SUpdateContext& context, TDynArray<SSpawnEntry>& spawnEntries) {}

	virtual void InitParticles(const SUpdateContext& context) {}

	virtual void PostInitParticles(const SUpdateContext& context) {}

	virtual void KillParticles(const SUpdateContext& context, TConstArray<TParticleId> particleIds) {}

	virtual void DestroyParticles(const SUpdateContext& context) {}

	// Particle update
	virtual void PreUpdateParticles(const SUpdateContext& context) {}

	virtual void UpdateParticles(const SUpdateContext& context) {}

	virtual void PostUpdateParticles(const SUpdateContext& context) {}

	virtual void ComputeBounds(CParticleComponentRuntime* pComponentRuntime, AABB& bounds) {}

	// Rendering
	virtual void PrepareRenderObjects(CParticleEmitter* pEmitter, CParticleComponent* pComponent, bool bPrepare) {}
	virtual void Render(CParticleEmitter* pEmitter, CParticleComponentRuntime* pComponentRuntime, CParticleComponent* pComponent, const SRenderContext& renderContext) {}
	virtual void RenderDeferred(CParticleEmitter* pEmitter, CParticleComponentRuntime* pComponentRuntime, CParticleComponent* pComponent, const SRenderContext& renderContext) {}
	virtual void ComputeVertices(CParticleComponentRuntime* pComponentRuntime, const SCameraInfo& camInfo, CREParticle* pRE, uint64 uRenderFlags, float fMaxPixels) {}

	// GPU interface
	virtual void UpdateGPUParams(const SUpdateContext& context, gpu_pfx2::SUpdateParams& params) {}

	gpu_pfx2::IParticleFeature* GetGpuInterface() { return m_gpuInterface; }
	gpu_pfx2::IParticleFeature* MakeGpuInterface(CParticleComponent* pComponent, gpu_pfx2::EGpuFeatureType feature);

protected:
	void AddNoPropertiesLabel(Serialization::IArchive& ar);

private:
	SEnable                                m_enabled;
	_smart_ptr<gpu_pfx2::IParticleFeature> m_gpuInterface;
};

template<class... Args> using TFeatureDispatcher = Dispatcher<CParticleFeature, Args...>;

struct SFeatureDispatchers
{
	TFeatureDispatcher<CParticleComponentRuntime*> MainPreUpdate { &CParticleFeature::MainPreUpdate };

	TFeatureDispatcher<const SUpdateContext&, SUpdateRange> InitSubInstances { &CParticleFeature::InitSubInstances };
	TFeatureDispatcher<const SUpdateContext&, TDynArray<SSpawnEntry>&> SpawnParticles { &CParticleFeature::SpawnParticles };

	TFeatureDispatcher<const SUpdateContext&, TConstArray<float>, TVarArray<float>> GetSpatialExtents { &CParticleFeature::GetSpatialExtents };
	TFeatureDispatcher<const SUpdateContext&, TParticleId, Vec3&> GetEmitOffset { &CParticleFeature::GetEmitOffset };

	TFeatureDispatcher<const SUpdateContext&> InitParticles { &CParticleFeature::InitParticles };
	TFeatureDispatcher<const SUpdateContext&> PostInitParticles { &CParticleFeature::PostInitParticles };
	TFeatureDispatcher<const SUpdateContext&, TConstArray<TParticleId>> KillParticles { &CParticleFeature::KillParticles };
	TFeatureDispatcher<const SUpdateContext&> DestroyParticles { &CParticleFeature::DestroyParticles };

	TFeatureDispatcher<const SUpdateContext&> PreUpdateParticles { &CParticleFeature::PreUpdateParticles };
	TFeatureDispatcher<const SUpdateContext&> UpdateParticles { &CParticleFeature::UpdateParticles };
	TFeatureDispatcher<const SUpdateContext&> PostUpdateParticles { &CParticleFeature::PostUpdateParticles };

	TFeatureDispatcher<CParticleComponentRuntime*, AABB&> ComputeBounds { &CParticleFeature::ComputeBounds };
	TFeatureDispatcher<const SUpdateContext&, gpu_pfx2::SUpdateParams&> UpdateGPUParams { &CParticleFeature::UpdateGPUParams };

	TFeatureDispatcher<CParticleEmitter*, CParticleComponent*, bool> PrepareRenderObjects { &CParticleFeature::PrepareRenderObjects };
	TFeatureDispatcher<CParticleEmitter*, CParticleComponentRuntime*, CParticleComponent*, const SRenderContext&> Render { &CParticleFeature::Render };
	TFeatureDispatcher<CParticleEmitter*, CParticleComponentRuntime*, CParticleComponent*, const SRenderContext&> RenderDeferred { &CParticleFeature::RenderDeferred };
	TFeatureDispatcher<CParticleComponentRuntime*, const SCameraInfo&, CREParticle*, uint64, float> ComputeVertices { &CParticleFeature::ComputeVertices };
};

ILINE ColorB HexToColor(uint hex)
{
	return ColorB((hex >> 16) & 0xff, (hex >> 8) & 0xff, hex & 0xff);
}

static const ColorB colorAppearance = HexToColor(0x00ffba);
static const ColorB colorField      = HexToColor(0x02ba25);
static const ColorB colorRender     = HexToColor(0xc2fbbe);
static const ColorB colorLocation   = HexToColor(0x30a8fd);
static const ColorB colorAngles     = HexToColor(0x84d7fa);
static const ColorB colorSpawn      = HexToColor(0xfc7070);
static const ColorB colorLife       = HexToColor(0xd0c0ac);
static const ColorB colorKill       = HexToColor(0xd0c0ac);
static const ColorB colorVelocity   = HexToColor(0xcea639);
static const ColorB colorMotion     = HexToColor(0xfb9563);
static const ColorB colorLight      = HexToColor(0xfffdd0);
static const ColorB colorAudio      = HexToColor(0xd671f7);
static const ColorB colorGeneral    = HexToColor(0xececec);
static const ColorB colorChild      = HexToColor(0xc0c0c0);
static const ColorB colorProject    = HexToColor(0xc080c0);
static const ColorB colorGPU        = HexToColor(0x00e87e);
static const ColorB colorComponent  = HexToColor(0x80c0c0);

#define CRY_PFX2_DECLARE_FEATURE \
  struct SFeatureParams; \
  virtual const SParticleFeatureParams& GetFeatureParams() const override;

#define CRY_PFX2_IMPLEMENT_FEATURE_INTERNAL(BaseType, Type, GroupName, FeatureName, Color, UseConnector, DefaultForType) \
  struct Type::SFeatureParams: SParticleFeatureParams { SFeatureParams() {                                               \
    m_fullName = GroupName ": " FeatureName;                                                                             \
    m_groupName = GroupName;                                                                                             \
    m_featureName = FeatureName;                                                                                         \
    m_color = Color;                                                                                                     \
    m_pFactory = []() -> IParticleFeature* { return new Type(); };                                                       \
    m_hasComponentConnector = UseConnector;                                                                              \
    m_defaultForType = DefaultForType;                                                                                   \
  } };                                                                                                                   \
  const SParticleFeatureParams& Type::GetFeatureParams() const { static Type::SFeatureParams params; return params; }    \
  static bool sInit ## Type = CParticleFeature::RegisterFeature(Type::SFeatureParams());                                 \
  SERIALIZATION_CLASS_NAME(BaseType, Type, GroupName FeatureName, GroupName FeatureName);                                \
  SERIALIZATION_CLASS_NAME(IParticleFeature, Type, GroupName FeatureName, GroupName ":" FeatureName);                    \

#define CRY_PFX2_IMPLEMENT_FEATURE(BaseType, Type, GroupName, FeatureName, Color) \
  CRY_PFX2_IMPLEMENT_FEATURE_INTERNAL(BaseType, Type, GroupName, FeatureName, Color, false, 0)

#define CRY_PFX2_IMPLEMENT_FEATURE_DEFAULT(BaseType, Type, GroupName, FeatureName, Color, ForType) \
  CRY_PFX2_IMPLEMENT_FEATURE_INTERNAL(BaseType, Type, GroupName, FeatureName, Color, false, ForType)

#define CRY_PFX2_IMPLEMENT_FEATURE_WITH_CONNECTOR(BaseType, Type, GroupName, FeatureName, Color) \
  CRY_PFX2_IMPLEMENT_FEATURE_INTERNAL(BaseType, Type, GroupName, FeatureName, Color, true, 0)

#define CRY_PFX2_LEGACY_FEATURE(BaseType, NewType, LegacyName)           \
	SERIALIZATION_CLASS_NAME(BaseType, NewType, LegacyName, LegacyName);

}
