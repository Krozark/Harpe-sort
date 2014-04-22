namespace harpe
{
namespace learning
{
    bool Entity::operator>(const Entity& other)const
    {
        return (this->score == other.score)?this->size()<other.size():this->score > other.score;
    }
    
    float Entity::eval(const double * const values)const
    {
        return genome->eval(values);
    };
    
    bool Entity::need_eval()
    {
        return not evaluate;
    };

    int Entity::get_score()const
    {
        return score;
    }

    int Entity::size()const
    {
        return genome->nb_sub_nodes;
    }

    void Entity::minimize()
    {
        minimize(genome);
    }

    ///// NODE /////
    void Entity::Node::maj()
    {
        nb_sub_nodes = (fils1?fils1->nb_sub_nodes:0) + (fils2?fils2->nb_sub_nodes:0) + 1;
    }

    float Entity::Node::sin_f(float _1)
    {
        return sin(_1);
    }

    float Entity::Node::mul(float _1,float _2)
    {
        return _1*_2;
    }

    float Entity::Node::div(float _1,float _2)
    {
        return _1/_2;
    }

    float Entity::Node::add(float _1,float _2)
    {
        return _1+_2;
    }

    float Entity::Node::moins_b(float _1,float _2)
    {
        return _1-_2;
    }
}
}
