#include <iostream>
#include <string>
#include <ctime>
#include <queue>
using namespace std;
// Định nghĩa cấu trúc hoạt động
struct HoatDong
{
    unsigned int ID;
    string Ten;
    time_t ThoiGianBatDau;
    string ThongTin;
    int MucDoUuTien;
    HoatDong(int id, string ten, time_t tg, string tt, int mdut)
        : ID(id), Ten(ten), ThoiGianBatDau(tg), ThongTin(tt), MucDoUuTien(mdut) {}
    bool operator<(const HoatDong &other) const
    {

        if (MucDoUuTien == other.MucDoUuTien)
            return ThoiGianBatDau > other.ThoiGianBatDau;
        return MucDoUuTien < other.MucDoUuTien;
    }
};

// Định nghĩa node
struct Node
{
    HoatDong data;
    Node *left;
    Node *right;
    int do_cao;
    Node(HoatDong x) : data(x), left(nullptr), right(nullptr), do_cao(1) {}
};
int ChieuCao(Node *n);
int max(int a, int b);
int min(int a, int b);
Node *rightRotate(Node *y);
Node *leftRotate(Node *x);
void DeleteT(Node* &root, time_t tg);
void DelNode(Node* &node);
Node* CanBangCay(Node* root);
void XuLyThoiGianTrung(Node *node);
int LayHeSoCanBang(Node *n);
HoatDong *TimHoatDongXayRaSomNhat(Node *node);
void preOrder(Node *node);
time_t createTime(int year, int month, int day, int hour, int min);
void NhacNho(Node *root, const string &ten);
Node *insert(Node *node, HoatDong hd);

int main()
{
    HoatDong hd1(555, "Bai thi toan", createTime(2024, 12, 23, 22, 0), "Thi cuoi ki mon toan", 1);
    HoatDong hd2(666, "Hop nhom do an", createTime(2024, 12, 25, 8, 0), "Hop nhom ve do an", 2);
    HoatDong hd3(777, "Le hoi van hoa", createTime(2024, 12, 15, 6, 0), "Le hoi tai truong", 3);
    HoatDong hd4(888, "Di choi", createTime(2024, 12, 15, 6, 0), "Di choi voi gai", 4);

    Node *root = nullptr;
    root = insert(root, hd1);
    root = insert(root, hd2);
    root = insert(root, hd3);
    root = insert(root, hd4);
    preOrder(root);

    XuLyThoiGianTrung(root);

    // Gọi hàm để tìm hoạt động xảy ra sớm nhất
    HoatDong *somNhat = TimHoatDongXayRaSomNhat(root);
    if (somNhat != nullptr)
    {
        cout << "Hoat dong xay ra som nhat: " << somNhat->Ten
             << " , Thoi gian bat dau: " << ctime(&somNhat->ThoiGianBatDau) << endl;
    }
    else
    {
        cout << "Khong co hoat dong nao trong cay.\n";
    }
    
    NhacNho(root, "Bai thi toan");
    NhacNho(root, "Hop nhom do an");
    NhacNho(root, "Di an sinh nhat ban");
    return 0;
}

// Hàm lấy chiều cao của node
int ChieuCao(Node *n)
{
    return (n == nullptr) ? 0 : n->do_cao;
}

// Hàm lấy giá trị lớn hơn
int max(int a, int b)
{
    return (a > b) ? a : b;
}
int min(int a, int b)
{
    return (a < b) ? a : b;
}
// Xoay phải
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T = x->right;
    x->right = y;
    y->left = T;
    y->do_cao = max(ChieuCao(y->left), ChieuCao(y->right)) + 1;
    x->do_cao = max(ChieuCao(x->left), ChieuCao(x->right)) + 1;
    return x;
}

// Xoay trái
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T = y->left;
    y->left = x;
    x->right = T;
    x->do_cao = max(ChieuCao(x->left), ChieuCao(x->right)) + 1;
    y->do_cao = max(ChieuCao(y->left), ChieuCao(y->right)) + 1;
    return y;
}

void DeleteT(Node* &root, time_t tg) {
    if (root == nullptr)
        return;

    // Duyệt xuống trái hoặc phải để tìm node cần xóa
    if (tg < root->data.ThoiGianBatDau)
        DeleteT(root->left, tg);
    else if (tg > root->data.ThoiGianBatDau)
        DeleteT(root->right, tg);
    else {
        // Tìm thấy node cần xóa
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;

            // Node không có con hoặc có một con
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp; // Sao chép nội dung của node con lên node hiện tại
            }

            delete temp;
        } else {
            // Node có hai con, tìm node nhỏ nhất bên phải
            Node* temp = root->right;
            while (temp->left != nullptr)
                temp = temp->left;

            // Sao chép giá trị từ node nhỏ nhất bên phải
            root->data = temp->data;

            // Xóa node nhỏ nhất bên phải
            DeleteT(root->right, temp->data.ThoiGianBatDau);
        }
    }

    // Nếu cây chỉ có một node thì trả về
    if (root == nullptr)
        return;

    // Cập nhật chiều cao của node hiện tại
    root->do_cao = 1 + max(ChieuCao(root->left), ChieuCao(root->right));

    // Kiểm tra và cân bằng cây sau khi xóa
    root = CanBangCay(root);
}
void DelNode(Node* &node) {
    if (node != nullptr) {
        DeleteT(node, node->data.ThoiGianBatDau);
    }
}
Node* CanBangCay(Node* root) {
    // Bước 1: Cập nhật chiều cao của node hiện tại
    root->do_cao = 1 + max(ChieuCao(root->left), ChieuCao(root->right));

    // Bước 2: Tính hệ số cân bằng
    int balance = LayHeSoCanBang(root);

    // Bước 3: Kiểm tra và xử lý các trường hợp mất cân bằng

    // Left Left
    if (balance > 1 && LayHeSoCanBang(root->left) >= 0)
        return rightRotate(root);

    // Left Right
    if (balance > 1 && LayHeSoCanBang(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right
    if (balance < -1 && LayHeSoCanBang(root->right) <= 0)
        return leftRotate(root);

    // Right Left
    if (balance < -1 && LayHeSoCanBang(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


// Lấy hệ số cân bằng của cây
int LayHeSoCanBang(Node *n)
{
    return (n == nullptr) ? 0 : ChieuCao(n->left) - ChieuCao(n->right);
}

// Chèn một node vào cây AVL
Node *insert(Node *node, HoatDong hd)
{
    if (node == nullptr)
        return new Node(hd);
    if (hd.ID < node->data.ID)
        node->left = insert(node->left, hd);
    else if (hd.ID > node->data.ID)
        node->right = insert(node->right, hd);
    else
        return node;
    node->do_cao = 1 + max(ChieuCao(node->left), ChieuCao(node->right));
    int balance = LayHeSoCanBang(node);
    if (balance > 1 && hd.ID < node->left->data.ID)
        return rightRotate(node);
    if (balance < -1 && hd.ID > node->right->data.ID)
        return leftRotate(node);
    if (balance > 1 && hd.ID > node->left->data.ID)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && hd.ID < node->right->data.ID)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}
// xu ly thoi gian trung nhau140105
void XuLyThoiGianTrung(Node *node)
{
    if (node == nullptr)
        return;

    // So sánh node với con trái
    if (node->left && node->left->data.ThoiGianBatDau == node->data.ThoiGianBatDau)
    {
        cout << "Xung dot giua \"" << node->left->data.Ten << "\" va \"" << node->data.Ten << "\"\n";
        if (node->left->data.MucDoUuTien > node->data.MucDoUuTien)
           // cout << "Huy su kien \"" << node->left->data.Ten << "\"\n";
            DelNode(node->left);
        else
          //  cout << "Huy su kien \"" << node->data.Ten << "\"\n";
            DelNode(node);
    }

    // So sánh node với con phải
    if (node->right && node->right->data.ThoiGianBatDau == node->data.ThoiGianBatDau)
    {
        cout << "Xung dot giua \"" << node->right->data.Ten << "\" va \"" << node->data.Ten << "\"\n";
        if (node->right->data.MucDoUuTien > node->data.MucDoUuTien)
           // cout << "Huy su kien \"" << node->right->data.Ten << "\"\n";
            DelNode(node->right);
        else
           // cout << "Huy su kien \"" << node->data.Ten << "\"\n";
           DelNode(node);
    }
    XuLyThoiGianTrung(node->left);
    XuLyThoiGianTrung(node->right);
}

// Tìm hoat dong xay ra som nhat
HoatDong *TimHoatDongXayRaSomNhat(Node *node)
{
    if (node == nullptr)
        return nullptr;

    HoatDong *left = TimHoatDongXayRaSomNhat(node->left);
    HoatDong *right = TimHoatDongXayRaSomNhat(node->right);

    HoatDong *minHoatDong = &node->data;
    if (left && left->ThoiGianBatDau < minHoatDong->ThoiGianBatDau)
        minHoatDong = left;
    if (right && right->ThoiGianBatDau < minHoatDong->ThoiGianBatDau)
        minHoatDong = right;

    return minHoatDong;
}

// Duyệt cây theo thứ tự trước
void preOrder(Node *node)
{
    if (node != nullptr)
    {
        cout << node->data.ID << " : " << node->data.Ten << " : " << ctime(&node->data.ThoiGianBatDau);
        preOrder(node->left);
        preOrder(node->right);
    }
}

// nhac nho su kien
void NhacNho(Node *root, const string &ten)
{
    if (!root)
    {
        cout << "khong tim thay hoat dong \"" << ten << "\".\n";
        return;
    }

    // Duyệt cây để tìm hoạt động theo tên
    if (root->data.Ten == ten)
    {
        time_t currentTime = time(0); // Thời gian hiện tại
        double secondsLeft = difftime(root->data.ThoiGianBatDau, currentTime);

        if (secondsLeft > 0)
        {
            cout << "Nhac nho hoat dong: \"" << ten << "\" se dien ra trong "
                 << secondsLeft / 60 << " phut.\n";
        }
        else
        {
            cout << "Hoat dong \"" << ten << "\" da ket thuc.\n";
        }
        return;
    }
    if (ten != root->data.Ten)
    {
        NhacNho(root->left, ten);
    }
    else
    {
        NhacNho(root->right, ten);
    }
}

// Hàm tạo thời gian
time_t createTime(int year, int month, int day, int hour, int min)
{
    struct tm t = {0};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = min;
    t.tm_sec = 0;
    return mktime(&t);
}

// Chương trình chính

