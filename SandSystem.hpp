#ifndef SAND_SYSTEM_H_
#define SAND_SYSTEM_H_

#include "ParticleState.hpp"
#include "ParticleSystemBase.hpp"
#include "PendulumSpring.hpp"
#include "glm/gtx/string_cast.hpp"


namespace GLOO {
    class SandSystem : ParticleSystemBase
    {
    public:
        std::vector<float> particle_masses;
        std::vector<PendulumSpring> springs;
        std::vector<bool> particles_fixed;
        glm::vec3 g = glm::vec3(0.0f, - 1.14f, 0.0f);
        float k = .5f;

        ParticleState ComputeTimeDerivative(const ParticleState& state,
            float time) const {
            std::vector<glm::vec3> positions = state.positions;
            std::vector<glm::vec3> velocities = state.velocities;
            ParticleState new_state;
            new_state.positions = state.velocities;
            for (int i = 0; i < state.velocities.size(); i++) {
                glm::vec3 G = particle_masses[i] * g;
                new_state.velocities.push_back((G) / particle_masses[i]);
            }

            for (int i = 0; i < particles_fixed.size(); i++) {
                if (positions[i][1] < -1.0f) {
                    new_state.positions[i] = glm::vec3(0, 0, 0);
                    new_state.velocities[i] = glm::vec3(0, 0, 0);
                }
                if (particles_fixed[i] == true) {
                    new_state.positions[i] = glm::vec3(0, 0, 0);
                    new_state.velocities[i] = glm::vec3(0, 0, 0);
                }
            }

            for (int i = 0; i < new_state.positions.size(); i++) {
                //std::cout << glm::to_string(new_state.positions[i]) << std::endl;
                //std::cout << glm::to_string(new_state.velocities[i]) << std::endl;
            }
            return new_state;
        }

        void addParticle(float mass) {
            particle_masses.push_back(mass);
            particles_fixed.push_back(false);
        }
        void addSpring(int particle1, int particle2, float rest_length, float stiffness) {
            PendulumSpring spring = PendulumSpring(particle1, particle2, rest_length, stiffness);
            springs.push_back(spring);
        }

        void fixParticle(int particle) {
            particles_fixed[particle] = true;
        }
    };
}
// namespace GLOO

#endif