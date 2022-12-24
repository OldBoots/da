#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    connect(ui->butt_path, SIGNAL(clicked()), SLOT(start_slot()));
    //    connect(ui->butt_check);
    connect(ui->butt_start, SIGNAL(clicked()), SLOT(start_slot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::read_data()
{
    start_no_term = ui->fild_start_symbol->text();
    if(start_no_term < 65 || start_no_term > 90){
        QMessageBox msgBox;
        msgBox.setText("The end.");
        msgBox.exec();
        return -1;

    }
    min = ui->fild_min->text().toInt();
    max = ui->fild_max->text().toInt();
    if(min > max || min < 0){
        QMessageBox msgBox;
        msgBox.setText("The end 1.");
        msgBox.exec();
        return -1;
    }
    QStringList row = ui->fild_rule->toPlainText().split("\n");
    QStringList var;
    for(int i = 0; i < row.size(); i++){
        list_no_term << row[i].left(1);
        var = row[i].right(row[i].size() - 3).split("|");
        for(int j = 0; j < var.size(); j++){
            list_rules << list_no_term[i] + ":" + var[j];
        }
    }
    user_chain = ui->fild_chain->text();
    qDebug() << "snt = " << start_no_term;
    qDebug() << min << " - " << max;
    qDebug() << list_no_term;
    qDebug() << list_rules;
    return 0;
}

void MainWindow::da(QChar symbol)
{
    list_var.clear();
    for (int i = 0; i < list_rules.size(); i++) {
        if(list_rules[i][0] == symbol){
            list_var << list_rules[i].right(list_rules[i].size() - 2);
        }
    }
}

void MainWindow::add_var(QString str){
    for (int i = 0; i < str.size(); i++) {
        if(str[str.size() - 1 - i] < 65 || str[str.size() - 1 - i] > 90){
            stek << symbol(str[str.size() - 1 - i], 1);
        }else{
            stek << symbol(str[str.size() - 1 - i], 0);
        }
    }
}

void MainWindow::del_var_stek(int size)
{
    qDebug() << "size var = " << size;
    for (int i = 0; i < size; i++) {
        stek.pop_back();
    }
}

void MainWindow::del_var_ans(int &index)
{
    for (int i = ans.size() - 1; i >= 0 && !stek.isEmpty(); i--) {
        if(!ans[i].term){
            del_var_stek(list_var[ans[i].var].size());
            stek << ans[i];
            stek[stek.size() - 1].var++;
            break;
        }
    }
    for (; ans[ans.size() - 1].term ;index--) {
        stek << ans[ans.size() - 1];
        ans.pop_back();
    }
    ans.pop_back();
}

bool MainWindow::check_stek_term(int index)
{
    for (int i = stek.size() - 1, j = index; i >= 0; i--) {
        if(stek[i].term){
            if(stek[i].symb != user_chain[j]){
                return false;
            }
            j++;
        }
    }
    return true;
}

bool MainWindow::check_vec_noterm(QVector<symbol> vec)
{
    for(int i = 0 ; i < vec.size(); i++){
        if(!vec[i].term){
            return true;
        }
    }
    return false;
}

bool MainWindow::check_cont_chain(int index)
{
    QString str;
    for(int i = stek.size() - 1 ; i >= 0; i--){
        if(stek[i].term){
            str += stek[i].symb;
        }
    }
//    qDebug() << "UC = " << user_chain.right(user_chain.size() - index);
//    qDebug() << "str = " << str;
//    qDebug() << "i = " << index;
    if(user_chain.right(user_chain.size() - index).contains(str)){
        return true;
    }
    return false;
}

bool MainWindow::check_ans_chain()
{
    QString str;
    for (int i = 0; i < ans.size(); i++) {
        if(ans[i].term){
            str += ans[i].symb;
        }
    }
    if(user_chain == str){
        return true;
    }
    return false;
}

bool MainWindow::check_end()
{
    for(int i = 0; i < stek.size(); i++){
        if(!stek[i].term ){
            da(stek[i].symb);
            if(stek[i].var < list_var.size()){
                return true;
            }
        }
    }
    return false;
}

bool MainWindow::fall_back(int &i)
{
    if(stek.isEmpty()){
        if(check_ans_chain()){
            qDebug("eeeee");
            return false;
        }else{
            qDebug("Huy");
            while(ans[ans.size() - 1].term){
                if(ans[ans.size() - 1].symb != "~"){
                    i--;
                }
                stek << ans[ans.size() - 1];
                ans.pop_back();
            }
            if(check_vec_noterm(ans) && (!ans.isEmpty() || check_end())){
                da(ans[ans.size() - 1].symb);
                del_var_stek(list_var[ans[ans.size() - 1].var].size());
                ans[ans.size() - 1].var++;
                stek << ans[ans.size() - 1];
                ans.pop_back();
            }else{
                qDebug("The end 2.");
                return false;
            }
        }
    }
    return true;
}

QString MainWindow::print_vec_symb(QVector<symbol> vec)
{
    QString str;
    for (auto &symb : vec) {
        if(symb.term){
            str += symb.symb;
        }else{
            str += (QString)symb.symb + "(" + QString::number(symb.var) + ")";
        }
    }
    return str;
}

void MainWindow::clear_all_data()
{
    start_no_term.clear();
    stek.clear();
    ans.clear();
    user_chain.clear();
    list_rules.clear();
    list_no_term.clear();
    ui->fild_out->clear();
}

void MainWindow::analysis_chain_slot()
{
    stek << symbol(start_no_term[0], 0);
    for (int i = 0; i <user_chain.size();) {
        if(stek[stek.size() - 1].term){
            if(check_vec_noterm(stek)){
                ans << stek[stek.size() - 1];
                stek.pop_back();
                if(ans[ans.size() - 1].symb != "~"){
                    i++;
                }
                if(!fall_back(i)){
                    break;
                }
            }else{
                if(check_stek_term(i)){
                    ans << stek[stek.size() - 1];
                    stek.pop_back();
                    if(ans[ans.size() - 1].symb != "~"){
                        i++;
                    }
                    if(!fall_back(i)){
                        break;
                    }
                }else{
                    for (int i = ans.size() - 1; i >= 0 && !stek.isEmpty(); i--) {
                        if(!ans[i].term){
                            del_var_stek(list_var[ans[i].var].size());
                            stek << ans[i];
                            stek[stek.size() - 1].var++;
                            break;
                        }
                    }
                    ans.pop_back();
                }
            }
        }else{
            da(stek[stek.size() - 1].symb);
            if(stek[stek.size() - 1].var >= list_var.size() || !check_cont_chain(i)){
                qDebug("Huy");
                if(ans.isEmpty() && !check_end()){
                    qDebug("huyn'a");
                    break;
                }
                while(ans[ans.size() - 1].term){
                    qDebug("Huy1");
                    if(){
                        i--;
                    }
                    stek << ans[ans.size() - 1];
                    ans.pop_back();
                }
                if(check_vec_noterm(ans)){
                    da(ans[ans.size() - 1].symb);
                    del_var_stek(list_var[ans[ans.size() - 1].var].size());
                    ans[ans.size() - 1].var++;
                    stek << ans[ans.size() - 1];
                    ans.pop_back();
                }else{
                    qDebug("The end 2.");
                    break;
                }
            }else{
                ans << stek[stek.size() - 1];
                stek.pop_back();
                da(ans[ans.size() - 1].symb);
                add_var(list_var[ans[ans.size() - 1].var]);
            }
        }
        ui->fild_out->append("(Q," + QString::number(i) + "," + print_vec_symb(ans) + ", "  + print_vec_symb(stek) + ")");
        qDebug() << "(Q," + QString::number(i) + "," + print_vec_symb(ans) + ", "  + print_vec_symb(stek) + ")";
    }
    ui->fild_out->append("(Q," + QString::number(user_chain.size() - 1) + "," + print_vec_symb(ans) + ", "  + print_vec_symb(stek) + ")");
    qDebug() << "(Q," + QString::number(user_chain.size() - 1) + "," + print_vec_symb(ans) + ", "  + print_vec_symb(stek) + ")";
}

void MainWindow::start_slot()
{
    clear_all_data();
    read_data();
    analysis_chain_slot();
}
