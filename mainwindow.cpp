#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include<string>
#include"FASTAparser.h"
#include"DNAsequence.h"
#include<QMessageBox>
#include"Aligner.h"
#include"CSVLoader.h"
#include"DESeq2Result.h"
#include"DESeq2Analyzer.h"
#include"Resultloader.h"
#include"Painter.h"

//using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("生物工程小应用");
    connect(ui->action,&QAction::triggered,this,&MainWindow::saveSlot);
    choice=0;


}
void printAnalysis(QTextEdit*textEdit,string filename);
void printAlign(QTextEdit*textEdit,string filename_1,string filename_2);
void printDeseq2result(QTextEdit*textEdit,string filename);


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveSlot()
{
    if(choice==0){
        QMessageBox::warning(this,"错误","尚未进行分析");
    }

    else if(choice==1){
        QString fileName=QFileDialog::getSaveFileName(this,"保存为",
                                                        "AnalysisResult.txt","*.txt");
        // if(fileName.isEmpty()){
        //     QMessageBox::warning(this,"警告","请选择一个文件");
        // }
        {
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            QByteArray ba;
            //ba.append(qPrintable(ui->textEdit->toPlainText()));
            ba.append(ui->textEdit->toPlainText().toUtf8());
            file.write(ba);
        }
    }
    else if(choice==2){
        QString fileName=QFileDialog::getSaveFileName(this,"保存为",
                                                        "AlignResult.html","*.html");
        // if(fileName.isEmpty()){
        //     QMessageBox::warning(this,"警告","请选择一个文件");
        // }
        {
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            QByteArray ba;
            ba.append(htmlContent.toUtf8());
            file.write(ba);
        }
    }
    else if(choice==3){
        QString fileName=QFileDialog::getSaveFileName(this,"保存为",
                                                        "Deseq2Result.csv","*.csv");
        // if(fileName.isEmpty()){
        //     QMessageBox::warning(this,"警告","请选择一个文件");
        // }
        {
            QFile file(fileName);
            file.open(QIODevice::WriteOnly);
            QByteArray ba;
            ba.append(0xEF);
            ba.append(0xBB);
            ba.append(0xBF);
            ba.append(ui->textEdit->toPlainText().toUtf8());
            file.write(ba);
        }
    }
    else if(choice==4){
        QString fileName=QFileDialog::getSaveFileName(this,"保存为",
                                                        "VolcanoPlot.png","*.png");
        QPixmap pixmap(chart->size().toSize());
        pixmap.fill(Qt::white);  // 设置白色背景

        QPainter painter(&pixmap);
        chart->scene()->render(&painter);

        pixmap.save(fileName);
    }


}

void MainWindow::on_pushButton_1_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("选择一个文件"),
                                                    "C:\\Users\\Lenovo\\Desktop",
                                                    ("*.fasta"));
    //qDebug()<<fileName;
    std::string path = fileName.toUtf8().toStdString();
    //qDebug()<<path;
    if(path.empty()){
        return;
    }
    else{
        choice=1;
        printAnalysis(ui->textEdit,path);
    }
}

void printAnalysis(QTextEdit*textEdit,string filename){
    if(filename.empty()){
        return;
    }
    FASTAparser parser;
    DNAsequence seq=parser.singleParse(filename);


    QString output;

    output += "序列信息:  " + seq.header + "\n";
    output += "序列长度:  " + QString::number(seq.length()) + "\n";
    output += "序列数据:  " + seq.sequence + "\n";
    output += "核苷酸成分：\n";

    // 使用 QString::arg() 进行格式化
    output += QString("A含量: %1 (%2%)").arg(seq.aAmount()).arg(seq.aContent() * 100.0, 0, 'f', 2) + "\n";
    output += QString("T含量: %1 (%2%)").arg(seq.tAmount()).arg(seq.tContent() * 100.0, 0, 'f', 2) + "\n";
    output += QString("C含量: %1 (%2%)").arg(seq.cAmount()).arg(seq.cContent() * 100.0, 0, 'f', 2) + "\n";
    output += QString("G含量: %1 (%2%)").arg(seq.gAmount()).arg(seq.gContent() * 100.0, 0, 'f', 2) + "\n";
    output += QString("未知序列N含量: %1 (%2%)").arg(seq.nAmount()).arg(seq.nContent() * 100.0, 0, 'f', 2) + "\n";
    output += QString("GC含量: %1%").arg(seq.gcContent() * 100.0, 0, 'f', 2) + "\n";

    // 将结果输出到 QTextEdit
    textEdit->setPlainText(output);  // 一次性设置全部文本

}


void MainWindow::on_pushButton_2_clicked()
{
    QString fileName_1 = QFileDialog::getOpenFileName(this, ("选择1号比对文件"),
                                                    "C:\\Users\\Lenovo\\Desktop",
                                                    ("*.fasta"));
    std::string path_1 = fileName_1.toUtf8().toStdString();
    if(path_1.empty())
        return;
    QString fileName_2 = QFileDialog::getOpenFileName(this, ("选择2号比对文件"),
                                                      "C:\\Users\\Lenovo\\Desktop",
                                                      ("*.fasta"));
    std::string path_2 = fileName_2.toUtf8().toStdString();
    if(path_2.empty())
        return;
    choice=2;
    printAlign(path_1,path_2);
}

void MainWindow::printAlign(string filename_1,string filename_2){
    FASTAparser parser;
    DNAsequence seq1=parser.singleParse(filename_1);
    DNAsequence seq2=parser.singleParse(filename_2);
    Aligner aligner;
    auto alignmentresult=aligner.NeedlemanWunch(seq1,seq2);

    QString htmlTemplate = QString::fromUtf8(u8R"(<html><head>
    <meta charset="UTF-8">
    <title>AlignmentResult</title>
</head>
<body style="margin: 0; font-family: Courier New; padding: 20px;">
    <h2 align="center">序列比对结果</h2>
    <h3>被比对序列信息：</h3>
    <div>
        <p>%1<br>序列长度：%2</p>
        <p>%3<br>序列长度：%4</p>
    </div>
    <h3>比对结果:</h3>
    <div class="scroll-container" style="overflow-x: auto;max-width: 100%; border: 1px solid #ddd;padding: 15px;line-height:0;">
        <div class="line" style="font-family:Courier New; white-space: pre; margin: 0px 0; padding: 6px; line-height:7px;font-size:24px">%5</div>
        <div class="line" style="font-family:Courier New; white-space: pre; margin: 0px 0; padding: 6px; line-height:7px;font-size:24px">%6</div>
        <div class="line" style="font-family:Courier New; white-space: pre; margin: 0px 0; padding: 6px; line-height:7px;font-size:24px">%7</div>
    </div>
</body></html>)");

    htmlContent = htmlTemplate
                              .arg(alignmentresult.seq1header)
                              .arg(alignmentresult.seq1lenth)
                              .arg(alignmentresult.seq2header)
                              .arg(alignmentresult.seq2lenth)
                              .arg(alignmentresult.aligned_seq1)
                              .arg(alignmentresult.match_line)
                              .arg(alignmentresult.aligned_seq2);

    ui->textEdit->setHtml(htmlContent);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("选择一个文件"),
                                                      "C:\\Users\\Lenovo\\Desktop",
                                                      ("*.csv"));
    std::string path = fileName.toUtf8().toStdString();
    if(path.empty())
        return;
    choice=3;
    printDeseq2result(ui->textEdit,path);

}

void printDeseq2result(QTextEdit*textEdit,string filename){
    CSVLoader loader;
    CountData data = loader.loadFromCSV(filename);
    vector<DESeq2Result>results = DESeq2Analyzer().analyze(data,"HC","RA");
    QString output= "基因名称,log2fc,pvalue,padj\n";
    for(auto i:results){
        QString ct="%1,%2,%3,%4\n";
        output+=ct.arg(i.gene_name).arg(i.log2_fold_change).arg(i.p_value).arg(i.padj);
    }

    textEdit->setPlainText(output);
}

void MainWindow::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("选择一个文件"),
                                                    "C:\\Users\\Lenovo\\Desktop",
                                                    ("*.csv"));
    std::string path = fileName.toUtf8().toStdString();
    if(path.empty())
        return;
    choice=4;
    ui->textEdit->setPlainText("正在绘制中，请稍等...");
    QApplication::processEvents();
    printVolcanoPlot(path);
}

void MainWindow::printVolcanoPlot(string filename){
    ResultLoader loader;
    auto data=loader.readDESeq2Results(filename);
    Painter painter;
    chart = painter.createVolcanoPlotQt(data);
    QMainWindow* w2=new QMainWindow;
    w2->setWindowTitle("火山图");
    w2->resize(800, 600);
    QWidget *centralWidget = new QWidget(w2);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    // 将图表视图添加到布局
    layout->addWidget(chartView);
    // 设置中央部件
    w2->setCentralWidget(centralWidget);
    ui->textEdit->setPlainText("绘制完毕");
    QApplication::processEvents();
    w2->show();
    ui->textEdit->clear();
}

