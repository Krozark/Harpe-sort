namespace harpe
{
namespace learning
{
    bool Entity::operator>(const Entity& other)const
    {
        return (this->score == other.score)?this->size()<other.size():this->score > other.score;
    }
    
    double Entity::eval(const double * const values)const
    {
        return genome->eval(values);
    };
    
    bool Entity::need_eval()
    {
        return not evaluate;
    };

    double Entity::get_score()const
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

    double Entity::Node::sin_f(double _1)
    {
        return sin(_1);
    }

    double Entity::Node::mul(double _1,double _2)
    {
        return _1*_2;
    }

    double Entity::Node::div(double _1,double _2)
    {
        return _1/_2;
    }

    double Entity::Node::add(double _1,double _2)
    {
        return _1+_2;
    }

    double Entity::Node::moins_b(double _1,double _2)
    {
        return _1-_2;
    }
}
}
