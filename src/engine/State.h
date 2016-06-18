#ifndef INCLUDE_STATE_H
#define INCLUDE_STATE_H

namespace WalrusRPG
{
    namespace States
    {
        class State
        {
          public:
            virtual ~State(){};
            virtual void update() = 0;
            virtual void render() = 0;
        };
    }
}


#endif
