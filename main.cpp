#include "bst.hpp"

int main(){

    try{ 
        
        std::cout << "default ctor" << std::endl;
        std::cout << "insert" << std::endl;
        bst<int, int> tree;
        tree.insert({8,8});
        tree.insert({3,3});
        tree.insert({6,6});
        tree.insert({1,1});
        tree.insert({10,10});
        tree.insert({7,7});
        tree.insert({14,14});
        tree.insert({4,4});
        tree.insert({13,13});
        tree.draw();
        std::cout << std::endl;

        std::cout << "ctor with compare operator only" << std::endl;
        std::cout << "emplace function used with (k,v) or std::make_pair(k,v) " << std::endl;
        bst<std::string, std::string> students_tree{std::less<std::string>()};
        students_tree.emplace("Rossi","Rossi");
        students_tree.emplace("Verdi","Verdi");
        students_tree.emplace("Bianchi","Bianchi");
        students_tree.emplace(std::make_pair("Orzan","Orzan"));
        students_tree.emplace(std::make_pair("Sartori","Sartori"));
        students_tree.emplace(std::make_pair("Ruggeri","Ruggeri"));
        students_tree.draw();
        std::cout << std::endl;

        std::cout << "clear tree" << std::endl;
        tree.clear();
        std::cout << tree << std::endl;

        std::cout << "ctor with head and greater compare operator" << std::endl;
        std::cout << "[] operator" << std::endl;
        bst<int, int, std::greater<int>> eert{8,8,std::greater<int>()};
        eert[3] = 3;
        eert[6] = 6;
        eert[1] = 1;
        eert[10] = 10;
        eert[7] = 7;
        eert[14] = 14;
        eert[4] = 4;
        eert[13] = 13;
        eert.draw();
        std::cout << std::endl;

        std::cout << "ctor with all parameters and try inserting key already present" << std::endl;
        bst<int, int> tree_bis{8,8,std::less<int>()};;
        tree_bis.insert({3,3});
        tree_bis.insert({6,6});
        tree_bis.insert({1,1});
        tree_bis.insert({10,10});
        tree_bis.insert({7,7});
        tree_bis.insert({14,14});
        tree_bis.insert({4,4});
        tree_bis.insert({13,13});
        if(!tree_bis.insert({13,99}).second)
            std::cout << "key already present" << std::endl;
        tree_bis.draw();
        std::cout << std::endl;

        std::cout << "copy ctor copy_students_tree{students_tree}" << std::endl;
        bst<std::string, std::string> copy_students_tree{students_tree};
        std::cout << "students_tree: " << students_tree << std::endl;
        std::cout << "copy_students_tree: " << copy_students_tree << std::endl;
        std::cout << "insert into copied tree" << std::endl;
        copy_students_tree.insert({"Scordo","Scordo"});
        std::cout << "students_tree: " << students_tree << std::endl;
        std::cout << "copy_students_tree: " << copy_students_tree << std::endl;
        copy_students_tree.draw();
        std::cout << std::endl;

        students_tree.draw();
        std::cout << std::endl;
        std::cout << "balance above tree" << std::endl;
        students_tree.balance();
        students_tree.draw();
        std::cout << std::endl;

        std::cout << "copy assignment tree = tree_bis" << std::endl;
        tree = tree_bis;
        std::cout << "tree: " << tree << std::endl;
        std::cout << "tree_bis: " << tree_bis << std::endl;
        std::cout << "insert into copied tree" << std::endl;
        tree.insert({11,11});
        std::cout << "tree: " << tree << std::endl;
        std::cout << "tree_bis: " << tree_bis << std::endl;

        std::cout << "move assignment tree = std::move(tree_bis)" << std::endl;
        tree = std::move(tree_bis);
        std::cout << "tree: " << tree << std::endl;
        std::cout << "tree_bis: " << tree_bis << std::endl;
        std::cout << "move assignment tree_bis = std::move(tree)" << std::endl;
        tree_bis = std::move(tree);
        std::cout << "tree: " << tree << std::endl;
        std::cout << "tree_bis: " << tree_bis << std::endl << std::endl;

        std::cout << "move ctor tree_tris{std::move(tree_bis)}" << std::endl;
        bst<int, int> tree_tris{std::move(tree_bis)};
        std::cout << "tree_tris: " << tree_tris << std::endl;
        std::cout << "tree_bis: " << tree_bis << std::endl << std::endl;

        std::cout << "erase function all cases" << std::endl;
        tree_tris.draw();
        std::cout << std::endl;
        std::cout << "delete non existing key -> tree_tris.erase(20)" << std::endl;
        tree_tris.erase(20);
        std::cout << std::endl;
        std::cout << "delete node with 2 children -> tree_tris.erase(3)" << std::endl;
        tree_tris.erase(3);
        tree_tris.draw();
        std::cout << std::endl;
        std::cout << "delete node with only left child -> tree_tris.erase(14)" << std::endl;
        tree_tris.erase(14);
        tree_tris.draw();
        std::cout << std::endl;
        std::cout << "delete node with only right child -> tree_tris.erase(10)" << std::endl;
        tree_tris.erase(10);
        tree_tris.draw();
        std::cout << std::endl;
        std::cout << "delete leaf that is left child -> tree_tris.erase(1)" << std::endl;
        tree_tris.erase(1);
        tree_tris.draw();
        std::cout << std::endl;
        std::cout << "delete leaf that is right child -> tree_tris.erase(7)" << std::endl;
        tree_tris.erase(7);
        tree_tris.draw();
        std::cout << std::endl;

        std::cout << "erase function on root" << std::endl;
        std::cout << std::endl;

        std::cout << "t1" << std::endl;
        bst<int, int> t1{8,8,std::less<int>()};
        t1.insert({3,3});
        t1.draw();
        std::cout << std::endl;

        std::cout << "delete root with left child -> t1.erase(8)" << std::endl;
        t1.erase(8);
        t1.draw();
        std::cout << std::endl;

        std::cout << "t2" << std::endl;
        bst<int, int> t2{8,8,std::less<int>()};
        t2.insert({10,10});
        t2.draw();
        std::cout << std::endl;

        std::cout << "delete root with right child -> t2.erase(8)" << std::endl;
        t2.erase(8);
        t2.draw();
        std::cout << std::endl;

        std::cout << "t3" << std::endl;
        bst<int, int> t3{8,8,std::less<int>()};
        t3.insert({3,3});
        t3.insert({10,10});
        t3.draw();
        std::cout << std::endl;

        std::cout << "delete root with 2 children -> t3.erase(8)" << std::endl;
        t3.erase(8);
        t3.draw();
        std::cout << std::endl;

    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch(...) {
        std::cerr << "Unknown exception occured" << std::endl;
        return 2;
    }

    return 0;

}