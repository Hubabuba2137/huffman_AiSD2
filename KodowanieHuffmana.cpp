#include <iostream>
#include <string>
#include <algorithm> //swap

using namespace std;

//znak i ilość jego wystąpień
struct char_count {
    int wyst;
    char znak;
};

struct node_list {
   char_count sym;

   node_list* next;
   node_list* prev;

   node_list(char_count z) :sym(z), prev(nullptr), next(nullptr) {}
};

struct list {
   node_list* head;

   list() : head(nullptr) {}

   void add(char_count sym) {
       node_list* nowyElement = new node_list(sym);
       node_list* temp = new node_list(sym);

       if (head == nullptr) {
           head = temp;
       }

       else {
           int pozycja = 1;

           temp = head;
           while (temp->next != 0) {
               temp = temp->next;
               pozycja++;
           }

           if (pozycja == 0 || head == nullptr) {
               nowyElement->next = head;
               head = nowyElement;
           }
           else {
               node_list* poprzedni = loc(pozycja - 1);

               if (poprzedni != nullptr) {
                   nowyElement->next = poprzedni->next;
                   nowyElement->prev = poprzedni;
                   poprzedni->next = nowyElement;
               }
               else {
                   cout << "Błąd: Nie można dodać elementu na podanej pozycji." << std::endl;
                   delete nowyElement;
               }
           }
       }
   }

   void del(node_list* temp) {
       if (temp == nullptr) {
           cout << "Błąd: Nie można usunąć elementu z podanej pozycji." << std::endl;
       }

       else {
           if (temp->prev != nullptr) {
               temp->prev->next = temp->next;
           }
           else {
               head = temp->next;
           }

           if (temp->next != nullptr) {
               temp->next->prev = temp->prev;
           }

           delete temp;
       }

   }

   node_list* loc(int poz) {
       node_list* temp = head;
       int akt_poz = 0;

       while (temp != nullptr && akt_poz < poz) {
           temp = temp->next;
           akt_poz++;
       }

       return temp;
   }

   node_list* find(char c) {
       node_list* temp = head;

       while (temp != nullptr && temp->sym.znak != c) {
           temp = temp->next;
       }

       if (temp!= nullptr && temp->sym.znak == c) {
           return temp;
       }

       else {
           return nullptr;
       }
   }

    node_list* find(int value) {
       node_list* temp = head;

       while (temp != nullptr && temp->sym.wyst != value) {
           temp = temp->next;
       }

       if (temp!= nullptr && temp->sym.wyst == value) {
           return temp;
       }

       else {
           return nullptr;
       }
   }

   int size() {
         node_list* temp = head;
         int size = 0;
    
         while (temp != nullptr) {
              size++;
              temp = temp->next;
         }
    
         return size;
    }

   void print(node_list* temp) {
       while (temp != nullptr) {
           cout << temp->sym.znak<<": "<<temp->sym.wyst << " <-> ";
           temp = temp->next;
       }
   }
};

//węzeł drzewa huffmana
struct node {
    char znak;
    int wyst;
    node* left;
    node* right;
    node(char znak = '\0', int wyst = 0) : znak(znak), wyst(wyst), left(nullptr), right(nullptr) {}
};

struct huff_tree {
    node* root;
    huff_tree() : root(nullptr) {}
    huff_tree(node* root) : root(root) {}
};

//liczenie znaków w tekście
list counting(string text){
    list temp;

    bool znal = false;

    for(int i = 0; i < text.length(); i++){
        znal = false;
        node_list* temp2 = temp.head;

        while(temp2 != nullptr){
            if(temp2->sym.znak == text[i]){
                temp2->sym.wyst++;
                znal = true;
                break;
            }
            temp2 = temp2->next;
        }

        if(znal == false){
            char_count temp3;
            temp3.znak = text[i];
            temp3.wyst = 1;
            temp.add(temp3);
        }
    }

    return temp;
}

list sort(list tab) {
    for (size_t i = 0; i < tab.size(); ++i) {
        for (size_t j = 0; j < tab.size() - i - 1; ++j) {
            if (tab.loc(j)->sym.wyst > tab.loc(j + 1)->sym.wyst) {
                swap(tab.loc(j)->sym, tab.loc(j + 1)->sym);
            }
        }
    }
    return tab;
}

huff_tree* make_huff_tree(list tab) {
    while (tab.size() > 1) {
        tab = sort(tab);

        node_list* temp_p1 = tab.loc(0);
        node_list* temp_p2 = tab.loc(1);

        node* p1 = new node(temp_p1->sym.znak, temp_p1->sym.wyst);
        node* p2 = new node(temp_p2->sym.znak, temp_p2->sym.wyst);

        node* root = new node('\0', p1->wyst + p2->wyst);
        root->left = p1;
        root->right = p2;

        tab.del(temp_p1);
        tab.del(temp_p2);

        char_count temp;
        temp.znak = '\0';
        temp.wyst = root->wyst;
        tab.add(temp);
    }

    node* kor = new node(tab.head->sym.znak, tab.head->sym.wyst);

    huff_tree* tree = new huff_tree();
    tree->root = kor;

    return tree;
}

int main() {
    string text = "eloele";
    list tablica = counting(text);

    huff_tree* tree = make_huff_tree(tablica);

    return 0;
}
