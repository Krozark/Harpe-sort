#ifndef INDIVIDUTREE_HPP
#define _HPP

#include <cstdlib>
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <mutex>

#include <harpe-sort-learning/Spectrum.hpp>

#define TREE_INIT_PROONDEUR 7
#define TREE_SUB_INIT_PROFONDEUR 5

namespace harpe
{
namespace learning
{
    /**
     * \brief Classe utilisé par l'algorithme génétique représentant un arbre de fonction
     **/
    class Entity
    {
        public:
            static std::list<harpe::learning::Spectrum> learning_spectums;

            /**
             * \brief Constructeur
             **/
            Entity();//pour les vector, list ...

            /**
             * \brief Construit un Entity aléatoirement avec une profondeur maximal
             **/
            Entity(const int& profondeur_init);

            /**
             * \brief Copie explisitement
             **/
            Entity* clone() const;

            ~Entity();

            void mutate(); ///< fait mutter un coef en ajoutant [-1,1] ou remplacer sous arbre

            /**
             * \brief Fusionne avec other
             *
             * \param autre Entity avec qui fusionner
             **/
            Entity* crossOver(const Entity& other) const;

            void eval();///< fonction d'évaluation, affect le score

            inline double eval(const double * const)const;

            inline bool need_eval();///< pour le moteur, eviter les eval inutile

            /**
             * \brief Operateur de comparaison entre 2 individus
             **/
            inline bool operator>(const Entity& other)const;

            //void __print__();
            /**
             * \brief Fonction d'affichage de debug
             **/
            friend std::ostream& operator<<(std::ostream& output,const Entity& individu);

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

                    Node(double cst); ///< construit une constante
                    Node(int indice); ///< construit une valeur avec indice
                    Node(double (*funaire)(double),Node* _1); ///< construit une fonction unaire
                    Node(double (*funaire)(double,double),Node* _1,Node* _2); ///< construit une fonction binaire
                    ~Node();///< recursif

                    Node* clone() const;///< copie recursif
                    double eval(const double * const vals) const; ///< évalue et met le score

                    static Node* CreateRandTree(const int max_profondeur); ///< cré un arbre de manière aléatoire

                    /* FONCTIONS */
                    enum FUNCTIONS {
                        //unaire
                        SIN = 0,
                        //GAUSSIAN,
                        //binaire
                        MUL,
                        DIV,
                        ADD,
                        SUB,
                        SIZE
                    };

                    //croisement et mutation qui s'adapte au fur et à mesure du temps?
                    //nombre de croisement proportionele au nombre de nodes
                    //changer le nombre d'args à une fonctions auto définie
                    //petite mutation (un noeu de meme arité)

                    static int max_indice;///< nombre de case dans le tableau d'indice

                    friend std::ostream& operator<<(std::ostream& output,const Node* root); ///< affiche l'arbre

                private:
                    friend class Entity;
                    /**
                     * \brief Construit un Noeud
                     * Note : utilisation interne seulement
                     **/
                    Node();
                    inline void maj();///< mise à jour de nombre de fils pour un acces random en log(n)

                    Node* fils1; ///< fils 1
                    Node* fils2;///< fils 2

                    Type type; ///< type de noeud
                    /**
                     * \brief stocke les valeurs
                     **/
                    union
                    {
                        double value; ///< valeur
                        int indice; ///< indice
                        double (*funaire)(double _1); ///< pointeur sur fonctionunaire
                        double (*fbinaire)(double _1,double _2); ///< pointeur sur fonction binaire
                    };

                    int nb_sub_nodes; ///< nombre de fils.\n Note : 1 => feuilles
            };

            static float tauxMutation; ///< taux de miutation des nouveaux nés

            inline double get_score()const;///< \return de score
            inline int size()const;///< \return le nombre de noeud

            inline void minimize();///< fusionne les noeud constants

            //float seuil;//< TODO ??

        private:
            inline std::stack<Node*> get(int numero);///< entre ]1,genome->nb_sub_nodes[ , genome->nb_sub_nodes est le numero de la racine
            inline Node* get_node(int numero) const;///< entre ]1,genome->nb_sub_nodes[ , genome->nb_sub_nodes est le numero de la racine

            Node* genome; ///< noeud racine
            double score; ///< score

            void minimize(Node* root); ///< minimizer l'arbre

            bool evaluate; ///< à été avalué
    };
}
}
#include <harpe-sort-learning/Entity.tpl>
#endif
