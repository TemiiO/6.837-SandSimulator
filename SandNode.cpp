#include "SandNode.hpp"
#include "ParticleSystemBase.hpp"
#include "IntegratorFactory.hpp"

#include "gloo/debug/PrimitiveFactory.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/shaders/PhongShader.hpp"
#include "gloo/shaders/SimpleShader.hpp"
#include "gloo/InputManager.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include <string>


namespace GLOO {
    SandNode::SandNode(IntegratorType type, float step) {
        integrator = IntegratorFactory::CreateIntegrator<SandSystem, ParticleState>(type);
        step_ = step;
        ParticleState first_state;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> velocities;
        radius_ = 0.015f;

        for (float i = 0; i < 30; i++) {
            //Modifying particle state
            positions.push_back(glm::vec3(0.0f, i / 2.0f, 0));
            velocities.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
            //Connecting the spring and fixing if its the first particle
            system_.addParticle(.80f);
            //Creating the scene node for the particle
            std::shared_ptr<VertexObject> sphere_mesh_ = PrimitiveFactory::CreateSphere(radius_, 25, 25);
            std::shared_ptr<PhongShader> shader = make_unique<PhongShader>();
            std::shared_ptr<VertexObject> mesh = sphere_mesh_;
            auto sphere = make_unique<SceneNode>();
            sphere->CreateComponent<ShadingComponent>(shader);
            sphere->CreateComponent<RenderingComponent>(sphere_mesh_);
            sphere->CreateComponent<MaterialComponent>(std::make_shared<Material>(Material::GetDefault()));
            glm::vec3 sand_stone = {0.689f, 0.565f, 0.510f};
            auto material_ptr = sphere->GetComponentPtr<MaterialComponent>();
            Material &material = material_ptr->GetMaterial();
            material.SetAmbientColor(sand_stone);
            material.SetDiffuseColor(sand_stone);
            sphere->GetComponentPtr<MaterialComponent>()->GetMaterial().SetDiffuseColor(sand_stone);
            sphere->GetTransform().SetPosition(glm::vec3(0.0f, i / 3.0f, 0));
            particleSpheres.push_back(sphere.get());
            AddChild(std::move(sphere));
        }

        first_state.positions = positions;
        first_state.velocities = velocities;
        state_ = first_state;
        //system_.fixParticle(0);
        time_ = 0.0f;
    }


    void SandNode::Update(double delta_time) {
        for (double i = 0; i < delta_time; i += step_) {
            ParticleState new_state = integrator->Integrate(system_, state_, time_ + i, step_);
            for (int j = 0; j < new_state.positions.size(); j++) {
                particleSpheres[j]->GetTransform().SetPosition(new_state.positions[j]);
            }
            state_ = new_state;
        }
        time_ += delta_time;
    }

}