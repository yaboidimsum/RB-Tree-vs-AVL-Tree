/*Dimas Prihady Setyawan NRP: 5025211184*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct AVLNode_t
{
    int data;
    struct AVLNode_t *kiri,*kanan;
    int tinggi;
}AVLNode;

typedef struct AVL_t
{
    AVLNode *_root;
    unsigned int _size;
}AVL;

AVLNode* _avl_createNode(int value) {
    AVLNode *newNode = (AVLNode*) malloc(sizeof(AVLNode));
    newNode->data = value;
    newNode->tinggi=1;
    newNode->kiri = newNode->kanan = NULL;
    return newNode;
}

AVLNode* _search(AVLNode *root, int value) {
    while (root != NULL) {
        if (value < root->data)
            root = root->kiri;
        else if (value > root->data)
            root = root->kanan;
        else
            return root;
    }
    return root;
}

int _getTinggi(AVLNode* node){
    if(node==NULL)
        return 0;
    return node->tinggi;
}

int _max(int a,int b){
    return (a > b)? a : b;
}

AVLNode* _kananRotate(AVLNode* pivotNode) {

    AVLNode* newParrent=pivotNode->kiri;
    pivotNode->kiri=newParrent->kanan;
    newParrent->kanan=pivotNode;

    pivotNode->tinggi=_max(_getTinggi(pivotNode->kiri),
                      _getTinggi(pivotNode->kanan))+1;
    newParrent->tinggi=_max(_getTinggi(newParrent->kiri),
                       _getTinggi(newParrent->kanan))+1;

    return newParrent;
}

AVLNode* _kiriRotate(AVLNode* pivotNode) {

    AVLNode* newParrent=pivotNode->kanan;
    pivotNode->kanan=newParrent->kiri;
    newParrent->kiri=pivotNode;

    pivotNode->tinggi=_max(_getTinggi(pivotNode->kiri),
                      _getTinggi(pivotNode->kanan))+1;
    newParrent->tinggi=_max(_getTinggi(newParrent->kiri),
                       _getTinggi(newParrent->kanan))+1;

    return newParrent;
}

AVLNode* _kiriCaseRotate(AVLNode* node){
    return _kananRotate(node);
}

AVLNode* _kananCaseRotate(AVLNode* node){
    return _kiriRotate(node);
}

AVLNode* _kiriKananCaseRotate(AVLNode* node){
    node->kiri=_kiriRotate(node->kiri);
    return _kananRotate(node);
}

AVLNode* _kananKiriCaseRotate(AVLNode* node){
    node->kanan=_kananRotate(node->kanan);
    return _kiriRotate(node);
}

int _getBalanceFactor(AVLNode* node){
    if(node==NULL)
        return 0;
    return _getTinggi(node->kiri)-_getTinggi(node->kanan);
}

AVLNode* _tambah_AVL(AVL *avl,AVLNode* node,int value) {

    if(node==NULL)
        return _avl_createNode(value);
    if(value < node->data)
        node->kiri = _tambah_AVL(avl,node->kiri,value);
    else if(value > node->data)
    	node->kanan = _tambah_AVL(avl,node->kanan,value);

    node->tinggi= 1 + _max(_getTinggi(node->kiri),_getTinggi(node->kanan));

    int balanceFactor=_getBalanceFactor(node);

    if(balanceFactor > 1 && value < node->kiri->data)
        return _kiriCaseRotate(node);
    if(balanceFactor > 1 && value > node->kiri->data)
		return _kiriKananCaseRotate(node);
    if(balanceFactor < -1 && value > node->kanan->data)
        return _kananCaseRotate(node);
    if(balanceFactor < -1 && value < node->kanan->data)
        return _kananKiriCaseRotate(node);

    return node;
}

AVLNode* _cariMinNode(AVLNode *node) {
    AVLNode *currNode = node;
    while (currNode && currNode->kiri != NULL)
        currNode = currNode->kiri;
    return currNode;
}

AVLNode* _hapus_AVL(AVLNode* node,int value){
    if(node==NULL)
        return node;
    if(value > node->data)
    	node->kanan=_hapus_AVL(node->kanan,value);
    else if(value < node->data)
    	node->kiri=_hapus_AVL(node->kiri,value);
    else{
        AVLNode *temp;
        if((node->kiri==NULL)||(node->kanan==NULL)){
            temp=NULL;
            if(node->kiri==NULL) temp=node->kanan;
            else if(node->kanan==NULL) temp=node->kiri;

            if(temp==NULL){
                temp=node;
                node=NULL;
            }
            else
                *node=*temp;

            free(temp);
        }
        else{
            temp = _cariMinNode(node->kanan);
            node->data=temp->data;
            node->kanan=_hapus_AVL(node->kanan,temp->data);
        }
    }

	if(node==NULL) return node;

    node->tinggi=_max(_getTinggi(node->kiri),_getTinggi(node->kanan))+1;

    int balanceFactor= _getBalanceFactor(node);

    if(balanceFactor>1 && _getBalanceFactor(node->kiri)>=0)
        return _kiriCaseRotate(node);

    if(balanceFactor>1 && _getBalanceFactor(node->kiri)<0)
        return _kiriKananCaseRotate(node);

    if(balanceFactor < -1 && _getBalanceFactor(node->kanan)<=0)
        return _kananCaseRotate(node);

    if(balanceFactor < -1 && _getBalanceFactor(node->kanan)>0)
        return _kananKiriCaseRotate(node);

    return node;
}

void avl_init(AVL *avl) {
    avl->_root = NULL;
    avl->_size = 0u;
}

bool avl_isEmpty(AVL *avl) {
    return avl->_root == NULL;
}

bool avl_cari(AVL *avl, int value) {
    AVLNode *temp = _search(avl->_root, value);
    if (temp == NULL)
        return false;

    if (temp->data == value)
        return true;
    else
        return false;
}

void avl_tambah(AVL *avl,int value){
    if(!avl_cari(avl,value)){
        avl->_root=_tambah_AVL(avl,avl->_root,value);
        avl->_size++;
    }

}

void avl_hapus(AVL *avl,int value){
    if(avl_cari(avl,value)){
        avl->_root=_hapus_AVL(avl->_root,value);
        avl->_size--;
    }

}

void preorder(AVLNode *root) {
    if (root) {
        preorder(root->kiri);
        printf("%d ", root->data);
        preorder(root->kanan);
    }
}

void search_avl(AVL *avl,int value)
    {
        if(avl_cari(avl,value))
        {
            printf("[BERHASIL]Data %d ditemukan");
        }
        else
        {
            printf("[BERHASIL]Data tidak ditemukan");
        }
    }

int main(){
    AVL avltree;
    avl_init(&avltree);

    avl_tambah(&avltree, 10);
    avl_tambah(&avltree, 20);
    avl_tambah(&avltree, 30);
    avl_tambah(&avltree, 40);
    avl_tambah(&avltree, 50);

    preorder(avltree._root);
    printf("\n");

    avl_hapus(&avltree, 10);
    avl_hapus(&avltree, 50);

    preorder(avltree._root);







//    char command[50];
//    int angka,opt;



//    printf(" ====================AVL_TREE====================\n");
//    printf("1. Tambah\n");
//    printf("2. Hapus\n");
//    printf("3. Cari\n");
//    printf("4. Tampil\n");
//    printf("5. Keluar\n");
//
//    while(1){
//    printf("\n");
//    printf("Masukan perintah mu: ");
//    scanf("%d",&opt);
//        	switch(opt)
//        	{
//            case 1:
//                    printf("\nMasukkan nilai\n");
//        			scanf("%d", &angka);
//                    avl_tambah(&avltree, angka);
//
//                    printf("[BERHASIL]Data %d telah ditambahkan\n", angka);
//        			break;
//
//            case 2:
//                    printf("\nMasukkan nilai\n");
//                    scanf("%d", &angka);
//                    avl_hapus(&avltree, angka);
//
//                    printf("[BERHASIL]Data %d telah terhapus\n", angka);
//                    break;
//
//            case 3:
//                    printf("\nMasukkan nilai\n");
//                    scanf("%d", &angka);
//                    search_avl(&avltree,angka);
//                    break;
//            case 4:
//                    preorder(avltree._root);
//                    printf("\n");
//                    break;
//            case 5:
//                return 0;
//		}
//
//	}

}
