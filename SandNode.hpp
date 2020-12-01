#ifndef SAND_NODE_H_
#define SAND_NODE_H_


#include "gloo/SceneNode.hpp"
#include "SandSystem.hpp"
#include "IntegratorBase.hpp"
#include "IntegratorType.hpp"

namespace GLOO {
    class SandNode : public SceneNode
    {
    public:
        std::vector<SceneNode*> particleSpheres;
        std::unique_ptr<IntegratorBase<SandSystem, ParticleState>> integrator;
        ParticleState state_;
        SandSystem system_;
        float step_;
        float time_;
        SandNode(IntegratorType type, float step);
        virtual void Update(double delta_time) override;
    };
}
// namespace GLOO

#endif