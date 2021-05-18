//
//  main.cpp
//  algorithm_redblacktree
//
//  Created by 최희연 on 2021/05/10.
//

#include <iostream>
#include <string>
#include <stack>
using namespace std;

const int black = 0;
const int red = 1;

struct medicalRecord {    //진료기록을 담는 구조체
    string sickness;
    int expense;
    medicalRecord(string sickness, int expense) {
        this->sickness = sickness;
        this->expense = expense;
    }
};

struct patient {  // 환자 정보를 담는 구조체
    int patientNumber;
    string name;
    string phoneNumber;
    pair<int, int> address;
    stack<medicalRecord> mr;
    patient(int n, string name, string pN, int ax, int ay, string d, int c) {
        this->patientNumber = n;
        this->name = name;
        this->phoneNumber = pN;
        this->address.first = ax;
        this->address.second = ay;
        medicalRecord mrIni = medicalRecord(d, c);
        mr.push(mrIni);
    }
    patient();
};

struct node { //red black tree를 위한 노드 클래스
    int b;  // black or red
    int depth;  // 깊이
    node* l, * r, * parent;   // 왼쪽자식, 오른쪽 자식, 부모
    patient* p; // 환자 정보
    node(int bb, int nn, string name, string pN, int ax, int ay, string sick, int expense, class node* ll, class node* rr, class node* pp) {
        b = bb;
        p = new patient(nn, name, pN, ax, ay, sick, expense);
        l = ll;
        r = rr;
        parent = pp;
        depth = 0;
    }
    node() {}

};

class redBlackTree {
    node* root;
    node* leafNode;

public:
    redBlackTree() {
        leafNode = new node();
        leafNode->b = black;
        leafNode->l = nullptr;
        leafNode->r = nullptr;
        leafNode->parent = nullptr;
        root = leafNode;
    }

    node* getRoot() {
        return this->root;
    }

    // 환자 추가
    node* insert(int n, string name, string pN, int ax, int ay, string sick, int expense) {

        //
        node* x = getRoot();   // 삽입할 곳을 찾기 위함
        node* y = nullptr;  // 삽입할 곳의 부모노드
        node* z = search(n);    //tree에 해당 key가 이미 존재하는지 리턴

        if (z != nullptr && z != leafNode)    // 해당 환자가 이미 존재할 경우 nullptr반환
            return nullptr;


        else {   // 존재하지 않을 경우 삽입 연산 수행
            z = new node(red, n, name, pN, ax, ay, sick, expense, leafNode, leafNode, nullptr);
            while (x != leafNode) {
                y = x;

                if (x->p->patientNumber < n)
                    x = x->r;
                else
                    x = x->l;
            }

            z->parent = y;

            if (y == nullptr)
                root = z;
            else if (z->p->patientNumber > y->p->patientNumber)
                y->r = z;
            else
                y->l = z;

            //z가 root
            if (z->parent == nullptr) {
                depthFix(z);
                return z;
            }

            //z의 부모가 root
            if (z->parent->parent == nullptr) {
                depthFix(z);
                return z;
            }
            fixup(z);
            depthFix(root);
            return z;

        }


    }


    void fixup(node* z) {
        while (z != root && z->parent->b == red) {
            node* grand = z->parent->parent;
            node* uncle = (z->parent == grand->l) ? grand->r : grand->l;
            bool side = (z->parent == grand->l) ? true : false;
            //부모가 할아버지의 왼쪽자식이면 1, 오른쪽 자식이면 0

            //uncle이 있는데 uncle이 red이면 recoloring 수행
            if (uncle && uncle->b == red) {
                z->parent->b = black;
                uncle->b = black;
                grand->b = red;
                z = grand;
            }

            //uncle 없거나 uncle이 black일 경우 restructuring 수행
            else {
                // LR이거나 RL일 경우 : 회전 두번, 색상변경 해야함
                if (z == (side ? z->parent->r : z->parent->l)) {
                    z = z->parent;
                    side ? leftRotation(z) : rightRotation(z);
                }
                // LL이거나 RR일 경우 : 회전 한번, 색상변경 해야함

                z->parent->b = black;
                grand->b = red;
                side ? rightRotation(grand) : leftRotation(grand);

            }
        }
        root->b = black;
    }

    void leftRotation(node* v) {
        node* w = new node();

        w = v->r;
        v->r = w->l;
        w->l->parent = v;
        w->parent = v->parent;

        if (v->parent == nullptr)
            root = w;
        else if (v == v->parent->l)
            v->parent->l = w;
        else
            v->parent->r = w;

        v->parent = w;
        w->l = v;


    }


    void rightRotation(node* v) {
        node* w = new node();

        w = v->l;
        v->l = w->r;
        w->r->parent = v;
        w->parent = v->parent;

        if (v->parent == nullptr)
            root = w;
        else if (v == v->parent->l)
            v->parent->l = w;
        else
            v->parent->r = w;

        v->parent = w;
        w->r = v;
    }

    node* search(int patient_number) {
        node* x = root;
        node* parent = nullptr;

        while (x != nullptr && x != leafNode && x->p->patientNumber != patient_number) {
            parent = x;
            x = (patient_number < parent->p->patientNumber) ? parent->l : parent->r;
        }
        if (x == leafNode || x == nullptr)
            return nullptr;
        return x;
    }

    void depthFix(node* z) {
        if (z == leafNode)
            return;
        if (z == root)
            z->depth = 0;
        else if (z != leafNode)
            z->depth = z->parent->depth + 1;
        depthFix(z->l);
        depthFix(z->r);
    }

    int epidemic(string d, node* nowNode, int& cnt) {
        if (nowNode == leafNode)
            return cnt;
        if (nowNode->p->mr.top().sickness == d)  //
            cnt++;
        if (nowNode->l != leafNode)
            epidemic(d, nowNode->l, cnt);   //
        if (nowNode->r != leafNode)
            epidemic(d, nowNode->r, cnt);   //
        return cnt;

    }

    int addDisease(int k, string d, int c) {
        node* find = search(k);
        if (find == nullptr)
            return 0;
        else {
            medicalRecord mr = medicalRecord(d, c);
            find->p->mr.push(mr);
            return find->depth;
        }
    }

};

int main(int argc, const char* argv[]) {
    int T;
    scanf("%d", &T);
    redBlackTree* rbt = new redBlackTree();
    int k;  // 환자 번호
    string n;   // 환자 이름
    string h;   // 휴대폰 번호
    int ax, ay; // 주소
    string di;  // 병명
    int c;  // 진료비
    node* returnValue;
    for (int i = 0; i < T; i++) {
        char type; // I:신규가입, F:검색, A:추가진료, E:유행병 조사
        scanf("%c", &type);
        if (type == 'I') {
            scanf("%d %s %s %d %d %s %d", &k, &n, &h, &ax, &ay, &di, &c);
            returnValue = rbt->insert(k, n, h, ax, ay, di, c);
            if (returnValue == nullptr) {
                printf("%d 0 \n", rbt->search(k)->depth);
            }
            else {
                printf("%d 1 \n", returnValue->depth);
            }
        }
        else if (type == 'F') {
            scanf("%d", &k);
            returnValue = rbt->search(k);
            if (returnValue == nullptr)
                printf("Not found\n");
            else
                printf("%d %s %s %d %d\n", returnValue->depth, returnValue->p->name.c_str(), returnValue->p->phoneNumber.c_str(), returnValue->p->address.first, returnValue->p->address.second);
        }
        else if (type == 'A') {
            scanf("%d %s %d", &k, &di, &c);
            if (rbt->search(k) == nullptr)
                printf("Not found\n");
            else {
                printf("%d\n", rbt->addDisease(k, di, c));
            }
        }
        else if (type == 'E') {
            getline(cin, di);
            int cnt = 0;
            rbt->epidemic(di, rbt->getRoot(), cnt);
            printf("%d\n", cnt);
        }
        fflush(stdin);
    }
}
