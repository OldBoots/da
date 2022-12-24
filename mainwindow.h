#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMultiMap>
#include <QMouseEvent>
#include <QStringList>
#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class MainWindow;
}

struct symbol{
    QChar symb;
    bool term;
    int var;
    symbol(){}
    symbol(QChar s, bool t = 1){
        symb = s;
        term = t;
        var = 0;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    int read_data();
    bool eventFilter(QObject *obj, QEvent *event);
    void generation_chains();
    void get_rules_for_gen_chains();
    void da(QChar symbol);
    void add_var(QString str);
    void del_var_stek(int size);
    void del_var_ans(int &index);
    bool check_stek_term(int index);
    bool check_vec_noterm(QVector<symbol> vec);
    bool check_cont_chain(int index);
    bool check_ans_chain();
    bool check_end();
    bool fall_back(int &i);
    QString print_vec_symb(QVector <symbol> vec);
    void clear_all_data();
private slots:
    void analysis_chain_slot();
    void start_slot();
    void check_slot();
private:
    Ui::MainWindow *ui;
    QMultiMap<QString, QString> rules_map;
    QStringList list_no_term;
    QStringList list_rules;
    QVector <symbol> stek;
    QVector <symbol> ans;
    QStringList list_var;
    QString start_no_term;
    QString user_chain;
    QStandardItemModel model_chains;
    int min;
    int max;
};

#endif // MAINWINDOW_H
