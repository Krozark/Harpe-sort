#include <algorithm>
#include <functional>
#include <tuple>
#include <iostream>

#include <harpe-sort-learning/Entity.hpp>

int random(int,int)
{
    return 0;
}

int random(float,float)
{
    return 0;
}

namespace harpe
{
namespace learning
{
    std::list<harpe::learning::Spectrum> Entity::learning_spectums;

    Entity::Entity(): genome(0),score(0)/*, seuil(1.f)*/
    {
    };

    Entity::Entity(const int& profondeur_init): /*seuil(1),*/ score(0.f), evaluate(false)
    {
        genome = Node::CreateRandTree(profondeur_init);
    };

    Entity* Entity::clone() const
    {
        Entity* res = new Entity();
        res->genome = genome->clone();
        //res->seuil = seuil;
        res->score = score;
        res->evaluate = evaluate;
        return res;
    };

    Entity::~Entity()
    {
        delete genome;
    };


    void Entity::mutate()
    {
        evaluate = false;
        //tirer un node random
        std::stack<Node*> pile;

        if(genome->nb_sub_nodes > 1)
            pile = get(random(1,genome->nb_sub_nodes-1)); //entre 1 et la racine excluse
        else
            pile.push(genome);

        //lui appliquer une mutation (remplacement sous arbre, changement d'operateur, ou changer la valeur de indice / cst)
        Node* current = pile.top();
        bool recalc = false;

        if (random(0.f,1.f)<0.90) //grosse mutation (virer un sous arbre et le remplacer)
        {
            pile.pop();
            if (not pile.empty())//pas la racine
            {
                if(pile.top()->fils1 == current)
                    pile.top()->fils1= Node::CreateRandTree(TREE_SUB_INIT_PROFONDEUR);
                else
                    pile.top()->fils2= Node::CreateRandTree(TREE_SUB_INIT_PROFONDEUR);
                delete current;
            }
            else
            {
                genome = Node::CreateRandTree(TREE_INIT_PROONDEUR);
            }
            recalc = true;
        }
        else //petite (changement de type de node)
        {
            if(current->type == Node::CONSTANTE)
                current->value+= random(-1.f,1.f);//on ajoute un bruit
            else if(current->type == Node::VALS_INDICE)
                current->indice = random(0,Node::max_indice-1);//on change l'index
            else //fonctions
            {
                switch (random(0,4))
                {
                    /* unaire */
                    case 0:/* sin*/
                        current->type = Node::UNAIRE;
                        current->funaire = Node::sin_f;
                        break;
                    case 1:/*mul*/
                        current->type = Node::BINAIRE;
                        current->fbinaire = Node::mul;
                        if(not current->fils2)
                        {
                            current->fils2 = Node::CreateRandTree(TREE_SUB_INIT_PROFONDEUR);
                            recalc = true;
                        }
                        break;
                    case 2:/*div*/
                        current->type = Node::BINAIRE;
                        current->fbinaire = Node::div;
                        if(not current->fils2)
                        {
                            current->fils2 = Node::CreateRandTree(TREE_SUB_INIT_PROFONDEUR);
                            recalc = true;
                        }
                        break;
                    case 3:/*add*/
                        current->type = Node::BINAIRE;
                        current->fbinaire = Node::add;
                        if(not current->fils2)
                        {
                            current->fils2 = Node::CreateRandTree(TREE_SUB_INIT_PROFONDEUR);
                            recalc = true;
                        }
                        break;
                    case 4:/*moins_b*/
                        current->type = Node::BINAIRE;
                        current->fbinaire = Node::moins_b;
                        if(not current->fils2)
                        {
                            current->fils2 = Node::CreateRandTree(TREE_SUB_INIT_PROFONDEUR);
                            recalc = true;
                        }
                        break;
                }
            }
        }

        //mettre à jour le nombre de neuds
        if (recalc)
            while(not pile.empty())
            {
                current = pile.top();
                current->maj();
                pile.pop();
            }

    };

    Entity* Entity::crossOver(const Entity& other) const
    {
        Entity* res = new Entity;
        res->genome = this->genome->clone();
        res->evaluate = false;
        //res->seuil = this->seuil;

        std::stack<Node*> pile;
        if(res->genome->nb_sub_nodes > 1)
            pile = res->get(random(1,genome->nb_sub_nodes-1));
        else
            pile.push(res->genome);

        Node* other_node;
        if(other.genome->nb_sub_nodes > 1)
            other_node = other.get_node(random(1,other.genome->nb_sub_nodes-1))->clone();
        else
            other_node = other.genome->clone();


        Node* current = pile.top();
        pile.pop();

        if (not pile.empty())//pas la racine
        {
            if(pile.top()->fils1 == current)
                pile.top()->fils1= other_node;
            else
                pile.top()->fils2= other_node;
        }
        else
        {
            res->genome = other_node;
        }
        delete current;

        while(not pile.empty())
        {
            current = pile.top();
            current->maj();
            pile.pop();
        }

        return res;

    };

    std::ostream& operator<<(std::ostream& output,const Entity& individu)
    {
        output<<individu.genome;
        return output;
    };

    void Entity::eval()
    {
        this->score = 0;
        Entity& self = *this;
        for(Spectrum& spectrum : learning_spectums)
            self.score += spectrum.eval(self);
        evaluate = true;
    };

    void Entity::minimize(Entity::Node* root)
    {
        switch(root->type)
        {
            case Node::CONSTANTE:
            case Node::VALS_INDICE:
                return;
            case Node::UNAIRE:
                {
                    minimize(root->fils1);
                    if (root->fils1->type == Node::CONSTANTE)
                    {
                        root->value = root->eval(0);
                        root->type = Node::CONSTANTE;

                        delete root->fils1;
                        root->fils1 = 0;
                        if(root->fils2)
                        {
                            delete root->fils2;
                            root->fils2 = 0;
                        }
                        root->nb_sub_nodes = 1;
                    }
                    return;
                }
            case Node::BINAIRE:
                minimize(root->fils2);
                minimize(root->fils1);
                if (root->fils1->type == Node::CONSTANTE and root->fils2->type == Node::CONSTANTE)
                {
                    root->value = root->eval(0);
                    root->type = Node::CONSTANTE;

                    delete root->fils1;
                    root->fils1 = 0;
                    delete root->fils2;
                    root->fils2 = 0;

                    root->nb_sub_nodes = 1;
                }
                return;
        }

    };

    /* le nemero doit etre entre 1 et genome->nb_sub_nodes */
    std::stack<Entity::Node*> Entity::get(int numero)
    {
        /* soit l'arbre (récurcif):
         * X |-> Y
         *   |-> Z
         * On a alors:
         * N0(X) = X->nb_sub_nodes
         * N(Z) = N(X) - 1
         * N(Y) = N(Z) - Z->nb_sub_nodes
         * De cette facon chaque noeud parent a un numero plus grand que ces fils, et les numero vont entre [1 et genome->nb_sub_nodes]
         * renvoie la pile des parent(en cas le maj a faire sur les nb_sub_nodes)
         */
        std::stack<Node*> res;
        res.push(genome);
        int current_nb = genome->nb_sub_nodes;

        while(true)
        {
            if(numero >= current_nb) //on a trouvé
                return res;

            if(res.top()->fils2)//si il y a 2 fils, on regarde dans lequel des 2 il faut aller
            {
                int nb_max_gauche = current_nb-1- (res.top()->fils2->nb_sub_nodes);
                if(numero <= nb_max_gauche)
                {
                    current_nb = nb_max_gauche;
                    res.push(res.top()->fils1);
                }
                else
                {
                    --current_nb;
                    res.push(res.top()->fils2);
                }
            }
            else // sinon, on passe au suivant directement
            {
                res.push(res.top()->fils1);
                --current_nb;
            }
        }
        return res;
    };

    ///////////////////////// NODE ///////////////////////////////////
    Entity::Node::Node(): fils1(0),fils2(0)
    {
    };

    Entity::Node* Entity::get_node(int numero) const
    {
        Node* res = genome;
        int current_nb = res->nb_sub_nodes;

        while(true)
        {
            if(numero >= current_nb) //on a trouvé
                return res;

            if(res->fils2)//si il y a 2 fils, on regarde dans lequel des 2 il faut aller
            {
                int nb_max_gauche = current_nb-1- (res->fils2->nb_sub_nodes);
                if(numero <= nb_max_gauche)
                {
                    current_nb = nb_max_gauche;
                    res = res->fils1;
                }
                else
                {
                    --current_nb;
                    res = res->fils2;
                }
            }
            else // sinon, on passe au suivant directement
            {
                res = res->fils1;
                --current_nb;
            }
        }
        return res;
    };




    /* NODE */
    int Entity::Node::max_indice = 0;

    Entity::Node::Node(float cst) : type(Node::CONSTANTE), value(cst), nb_sub_nodes(1)
    {
        fils1= fils2 = 0;
    };

    Entity::Node::Node(int index) : type(Node::VALS_INDICE), indice(index), nb_sub_nodes(1)
    {
        fils1= fils2 = 0;
    };

    Entity::Node::Node(float(*f)(float),Node* _1) : type(Node::UNAIRE), funaire(f)
    {
        fils1 = _1;
        fils2 = 0;
        nb_sub_nodes = _1->nb_sub_nodes + 1;
    };

    Entity::Node::Node(float(*f)(float,float),Node* _1,Node* _2) : type(Node::BINAIRE), fbinaire(f)
    {
        fils1 = _1;
        fils2 = _2;
        nb_sub_nodes = _1->nb_sub_nodes + _2->nb_sub_nodes + 1;
    };

    Entity::Node::~Node()
    {
        if(fils1)
            delete fils1;
        if(fils2)
            delete fils2;
    };

    Entity::Node* Entity::Node::clone() const
    {
        //copie en profondeur
        Node* res = new Node();
        if(fils1)
            res->fils1 = fils1->clone();

        if(fils2)
            res->fils2 = fils2->clone();

        res->type = type;
        switch(type)
        {
            case Node::CONSTANTE : res->value = value;break;
            case Node::VALS_INDICE : res->indice = indice;break;
            case Node::UNAIRE : res->funaire = funaire;break;
            case Node::BINAIRE : res->fbinaire = fbinaire;break;
        }

        res->nb_sub_nodes = nb_sub_nodes;

        return res;
    };

    float Entity::Node::eval(const double * const vals) const
    {
        switch(type)
        {
            case Node::CONSTANTE : return value;
            case Node::VALS_INDICE : return vals[indice];
            case Node::UNAIRE : return funaire(fils1->eval(vals));
            case Node::BINAIRE : return fbinaire(fils1->eval(vals),fils2->eval(vals));
            default:
                std::cerr<<"Unknow type of node"<<std::endl;
                return 0;
        }
    };
    Entity::Node* Entity::Node::CreateRandTree(const int profondeur)
    {
        Node * res = NULL;
        if (profondeur <= 1 or random(0.f,1.f)> 0.9) //feuille
        {
            //creation d'une feuille aléatoire
            int choice = random(Node::CONSTANTE,Node::VALS_INDICE);
            switch (choice)
            {
                case Node::CONSTANTE :
                    res = new Node(random(0.f,1.f));
                    break;
                case Node::VALS_INDICE :
                    res = new Node(random(0,Node::max_indice-1));
                    break;
            }
        }
        else //autre node pour les fils
        {
            int choice = random(0,FUNCTIONS::SIZE);
            switch (choice)
            {
                /* unaire */
                case FUNCTIONS::SIN:/* sin*/
                    res = new Node(Node::sin_f,Node::CreateRandTree(profondeur-1));
                    break;
                    /*case 1://moin_u
                      res = new Node(Node::moins_u,Node::CreateRandTree(profondeur-1));
                      break;*/
                    /* binaire */
                case FUNCTIONS::MUL:/*mul*/
                    res = new Node(Node::mul,Node::CreateRandTree(profondeur-1),Node::CreateRandTree(profondeur-1));
                    break;
                case FUNCTIONS::DIV:/*div*/
                    res = new Node(Node::div,Node::CreateRandTree(profondeur-1),Node::CreateRandTree(profondeur-1));
                    break;
                case FUNCTIONS::ADD:/*add*/
                    res = new Node(Node::add,Node::CreateRandTree(profondeur-1),Node::CreateRandTree(profondeur-1));
                    break;
                case FUNCTIONS::MOINS_B:/*moins_b*/
                    res = new Node(Node::moins_b,Node::CreateRandTree(profondeur-1),Node::CreateRandTree(profondeur-1));
                    break;
            }
        }

        return res;
    };

    std::ostream& operator<<(std::ostream& output,const Entity::Node* root)
    {
        output<<"("; 
        switch(root->type)
        {
            case Entity::Node::CONSTANTE:
                {
                    output<<root->value;
                }break;
            case Entity::Node::VALS_INDICE :
                {
                    output<<"vals["<<root->indice<<"]";
                }break;
            case Entity::Node::UNAIRE:
                {
                    //operator
                    if(root->funaire == Entity::Node::sin_f)
                        output<<"sin";
                    /*else if (root->funaire == Entity::Node::moins_u)
                      output<<"-";*/
                    else
                        output<<"ERROR_UNAIRE";
                    output<<root->fils1;
                }break;
            case Entity::Node::BINAIRE:
                {
                    output<<root->fils1;
                    if(root->fbinaire == Entity::Node::mul)
                        output<<"*";
                    else if (root->fbinaire == Entity::Node::div)
                        output<<"/";
                    else if (root->fbinaire == Entity::Node::add)
                        output<<"+";
                    else if (root->fbinaire == Entity::Node::moins_b)
                        output<<"-";
                    else
                        output<<"ERROR_BINAIRE";
                    //operator
                    output<<root->fils2;
                }break;
        }
        output<<")"; 
        return output;
    };
}
}
