#include<iostream>
#include<vector>
#include<queue>
using namespace std;
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};
 
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> vv;
        int levelsize = 0;
        queue<TreeNode*> q;
        if (root) {
            q.push(root);
            levelsize = 1;
        }
        while (!q.empty()) {
            vector<int> v;

            while (levelsize--) { //当前层数据个数 控制一层一层的出
                TreeNode* node = q.front();
                q.pop();
                v.push_back(node->val); //结点的值push到vector容器里
                if (node->left) {
                    q.push(node->left);

                }
                if (node->right)
                    q.push(node->right);
            }
            vv.push_back(v); //把每一层的数据入进去
            levelsize = q.size(); //更新下一层数据的个数
        }
        return vv;
    }
};

//打印vector<vector<T>>
template<class Container>
void CoutContainer(Container v) {
    for (auto e : v)
    {
        for (auto b : e) {
            cout << b << " ";
        }
        cout << endl;
    }
   cout <<endl;
}
int main(){
    TreeNode n1(7);
    TreeNode n2(15);
    TreeNode n3(20,&n2,&n1);
    TreeNode n4(9);
    TreeNode n5(3,&n4,&n3);
    Solution s;
    vector<vector<int>> vv=s.levelOrder(&n5);
    CoutContainer(vv);
    return 0;
}