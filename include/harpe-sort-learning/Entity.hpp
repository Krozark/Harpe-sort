#ifndef HARPE_LEARNING_ENTITY_HPP
#define HARPE_LEARNING_ENTITY_HPP

namespace harpe
{
    namespace learning
    {
        class Entity
        {
            public:
                Entity();
                Entity(const Entity&) = delete;
                Entity& operator=(const Entity&) = delete;

            protected:

            private:
        };
    }
}
#endif