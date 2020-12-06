#ifndef SAND_SYSTEM_H_
#define SAND_SYSTEM_H_

#include "ParticleState.hpp"
#include "ParticleSystemBase.hpp"
#include "glm/gtx/string_cast.hpp"
#include <math.h>


namespace GLOO {
    class SandSystem : ParticleSystemBase
    {
    public:
        std::vector<float> particle_masses;
        std::vector<bool> particles_fixed;
        glm::vec3 g = glm::vec3(0.0f, -1.14f, 0.0f);
        float k = .5f;

        ParticleState ComputeTimeDerivative(const ParticleState& state,
            float time) const {
            std::vector<glm::vec3> positions = state.positions;
            std::vector<glm::vec3> velocities = state.velocities;
            ParticleState new_state;
            float mu = 0.4f;
            //Force of gravity on each particle
            for (int i = 0; i < state.velocities.size(); i++) {
                glm::vec3 G = particle_masses[i] * g;
                new_state.velocities.push_back(G);
            }

            //Collisions Code Here
            for (int i = 0; i < particle_masses.size(); i++) {
                //See if it collides with any other particle 
                glm::vec3 first_position = state.positions[i];
                for (int j = i + 1; j < particle_masses.size(); j++) {
                    glm::vec3 second_position = state.positions[j];
                    //Take these positions and the sphere radius - currently using .015f - and see if collides
                    float position_diff = glm::distance(first_position, second_position);
                    if (position_diff <= 0.015f) {
                        new_state.velocities[i] = -new_state.velocities[i];
                        new_state.velocities[j] = -new_state.velocities[j] + glm::vec3(0.5f, 0.5f, 0.0f);
                    }
                }
            }
            new_state.positions = state.velocities;

            //Fixed Particles and Handling the cutoff level
            for (int i = 0; i < particles_fixed.size(); i++) {
                //If the particle has hit the point where we don't want it to fall of the screen set the pos. and vel. to 0 in y-dir
                if (positions[i][1] < -1.0f) {
                    //Adding frictional force and making sure the particle stops vertically
                    glm::vec3 new_v = glm::vec3(0.0f);
                    glm::vec3 frictional_force = mu * particle_masses[i] * g;

                    if (new_state.velocities[i][0] > 0) {
                        if (std::abs(frictional_force[0]) < std::abs(new_state.velocities[i][0])) {
                            new_v = new_state.velocities[i] + frictional_force;
                            new_state.velocities[i] = new_v;
                        }
                        else {
                            new_state.positions[i][0] = 0.0f;
                        }
                    }
                    else {
                        if (std::abs(frictional_force[0]) < std::abs(new_state.velocities[i][0])) {
                            new_v = new_state.velocities[i] - frictional_force;
                            new_state.velocities[i] = new_v;
                        }
                        else {
                            new_state.positions[i][0] = 0.0f;
                        }
                    }
                    new_state.positions[i][1] = 0;
                    new_state.velocities[i][1] = 0;
                    new_state.positions[i][2] = 0;
                    new_state.velocities[i][2] = 0;
                }
            }

            return new_state;
        }

        void addParticle(float mass) {
            particle_masses.push_back(mass);
            particles_fixed.push_back(false);
        }

        void fixParticle(int particle) {
            particles_fixed[particle] = true;
        }
    };
}
// namespace GLOO

#endif