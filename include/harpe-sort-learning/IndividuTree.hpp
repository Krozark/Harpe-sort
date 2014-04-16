#ifndef INDIVIDUTREE_HPP
#define INDIVIDUTREE_HPP

#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>
#include <stack>

#include "../analyseur/analyseur_peptide.hpp"

#define TREE_INIT_PROONDEUR 7
#define TREE_SUB_INIT_PROFONDEUR 5
#define PRECISION_SORT 3

/**
 * \brief Classe utilisé par l'algorithme génétique représentant un arbre de fonction
 **/
class IndividuTree
{
    public:
        /**
         * \brief Constructeur
         **/
        IndividuTree(): genome(0),score(0)/*, seuil(1.f)*/{}; //pour les vector, list ...

        /**
         * \brief Construit un IndividuTree aléatoirement avec une profondeur maximal
         **/
        IndividuTree(const int& profondeur_init);

        /**
         * \brief Copie explisitement
         **/
        IndividuTree* clone() const;

        ~IndividuTree();

        void mutate(); ///< fait mutter un coef en ajoutant [-1,1] ou remplacer sous arbre

        /**
         * \brief Fusionne avec other
         *
         * \param autre IndividuTree avec qui fusionner
         **/
        IndividuTree* crossOver(const IndividuTree& other) const;

        void eval(); ///< fonction d'évaluation, affect le score

        static std::vector<AnalyseurPeptide*>* pep_to_test; ///< peptide lié
        bool need_eval(){return not evaluate;}; ///< pour le moteur, eviter les eval inutile

        /**
         * \brief Operateur de comparaison entre 2 individus
         **/
        bool operator>(const IndividuTree& other)const {return (this->score == other.score)?this->size()<other.size():this->score > other.score;}

        //void __print__();
        /**
         * \brief Fonction d'affichage de debug
         **/
        friend std::ostream& operator<<(std::ostream& output,const IndividuTree& individu);

        /**
         * \brief Classe représentant un noeud dans l'arbre ou une feuille
         **/
        class Node
        {
            public:
                /**
                 * \brief Type du neud
                 **/
                enum Type {
                    CONSTANTE=0, ///< une constante
                    VALS_INDICE, ///< l'indice d'une valeur dans le tableau en param
                    UNAIRE, ///< Fonction unaire
                    BINAIRE ///< Fonction binaire
                };

                Node(float cst); ///< construit une constante
                Node(int indice); ///< construit une valeur avec indice
                Node(float (*funaire)(float),Node* _1); ///< construit une fonction unaire
                Node(float (*funaire)(float,float),Node* _1,Node* _2); ///< construit une fonction binaire
                ~Node();///< recursif

                Node* clone() const;///< copie recursif
                float eval(const double * const vals) const; ///< évalue et met le score

                static Node* CreateRandTree(const int max_profondeur); ///< cré un arbre de manière aléatoire

                /* FONCTIONS */
                //unaire
                /*0*/static float sin_f(float _1) {return sin(_1);}; ///< fonction sinus
                //*6*/static float moins_u(float _1) {return -_1;};
                //binaire
                /*1*/static float mul(float _1,float _2) {return _1*_2;};///< fonction multiplié
                /*2*/static float div(float _1,float _2) {return _1/_2;}; ///< fonction divisé
                /*3*/static float add(float _1,float _2) {return _1+_2;}; ///< fonction addition
                /*4*/static float moins_b(float _1,float _2) {return _1-_2;};///< fonction de soustraction
                /*5 sous programme?*/

                //croisement et mutation qui s'adapte au fur et à mesure du temps?
                //nombre de croisement proportionele au nombre de nodes
                //changer le nombre d'args à une fonctions auto définie
                //petite mutation (un noeu de meme arité)
                
                static int max_indice;///< nombre de case dans le tableau d'indice

                friend std::ostream& operator<<(std::ostream& output,const Node* root); ///< affiche l'arbre

            private:
                friend class IndividuTree;
                /**
                 * \brief Construit un Noeud
                 * Note : utilisation interne seulement
                 **/
                Node(): fils1(0),fils2(0){};
                inline void maj(){nb_sub_nodes = (fils1?fils1->nb_sub_nodes:0) + (fils2?fils2->nb_sub_nodes:0) + 1;} ///< mise à jour de nombre de fils pour un acces random en log(n)

                Node* fils1; ///< fils 1
                Node* fils2;///< fils 2

                Type type; ///< type de noeud
                /**
                 * \brief stocke les valeurs
                 **/
                union
                {
                    float value; ///< valeur
                    int indice; ///< indice
                    float (*funaire)(float _1); ///< pointeur sur fonctionunaire
                    float (*fbinaire)(float _1,float _2); ///< pointeur sur fonction binaire
                };

                int nb_sub_nodes; ///< nombre de fils.\n Note : 1 => feuilles
        };

        static float tauxMutation; ///< taux de miutation des nouveaux nés

        const int get_score()const {return score;}; ///< \return de score
        const int size()const {return genome->nb_sub_nodes;}; ///< \return le nombre de noeud

        void minimize(){minimize(genome);}; ///< fusionne les noeud constants

        //float seuil;//< TODO ??

    private:
        inline std::stack<Node*> get(int numero);///< entre ]1,genome->nb_sub_nodes[ , genome->nb_sub_nodes est le numero de la racine
        inline Node* get_node(int numero) const;///< entre ]1,genome->nb_sub_nodes[ , genome->nb_sub_nodes est le numero de la racine

        int score; ///< score
        Node* genome; ///< noeud racine

        void minimize(Node* root); ///< minimizer l'arbre

        bool evaluate; ///< à été avalué
};

#endif
