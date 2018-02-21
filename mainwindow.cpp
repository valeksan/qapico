#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), m_pCore(new Core())
{
    ui->setupUi(this);

    ui->tableWidgetCurrencies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /* Первым делом необходимо создать объект для работы с базой данных
     * и инициализировать подключение к базе данных
     * */
    if(!isAppPathExists()) {
        qCritical() << "Missing application directory, and could not create it! (Access)";
    }

    db = new DataBase();
    db->connectToDataBase();

    /* Инициализация моделей */
    setupAppAreasModel();
    setupCurrenciesModel();
    setupGithubPoolModel();
    setupMarketPoolModel();
    setupRoadmapDatesModel();
    setupTechnologiesModel();
    setupTechPoolModel();
    setupTokenAlgModel();

    /* Регистрация нужных задач для выполнения ядром (мультипотоковым менеджером задач) */
    try {
        m_pCore->registerTask(Tasks::TASK_UPDATE, [this]() -> TaskResult
        {
            TaskResult result;
            // Скачивание страницы в текст
            Downloader html_downloader;
            connect(&html_downloader, &Downloader::onComplete, [=](QVariant data, int downloadType, int error) {
                                      Q_UNUSED(downloadType)
                                      Q_UNUSED(error)
                // формирование списка (парсинг)
                auto doc = QGumboDocument::parse(data.toString().toLatin1().data());
                auto root = doc.rootNode();
                auto tbody = root.getElementsByTagName(HtmlTag::TBODY).front();
                auto currency_nodes = tbody.getElementsByTagName(HtmlTag::TR);
                for(unsigned int index_node = 0; index_node < currency_nodes.size(); index_node++) {
                    //
                }
                qDebug() << "SIZE_TR: " << currency_nodes.size();
                //qDebug() << data.toString();
            });
            html_downloader.getData(QUrl("https://coinmarketcap.com/all/views/all/"));
            waitSignal(&html_downloader, &Downloader::onComplete, 10000);
            qDebug() << "UPDATE COMPLETE";
            return result;
        }, 0, 7000 /* Таймаут 7 сек. */);
    } catch (const CoreException &ex) {
        qDebug() << ex.message();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonUpdate_clicked()
{
    try {
        m_pCore->addTask(Tasks::TASK_UPDATE);
    } catch (const CoreException &ex) {
        qDebug() << ex.message();
    }
}

void MainWindow::setupCurrenciesModel()
{
    modelCurrencies = new QSqlRelationalTableModel(this);
    modelCurrencies->setTable(T_CURRENCIES);
    /* Устанавливаем связи с таблицей, по которым будет производится
         * подстановка данных
         * В метода setRelation указывается номер колонки, в которой будет
         * производится подстановка, а также с помощью класса
         * QSqlRelation указывается имя таблицы,
         * параметр, по которому будет произведена выборка строки
         * и колонка, из которой будут взяты данные
         * */
    modelCurrencies->setRelation(10, QSqlRelation(T_AREAS, "id", T_AREAS_NAME));
    modelCurrencies->setRelation(12, QSqlRelation(T_CURRENCIES, "id", T_CURRENCIES_NAME));
    modelCurrencies->setRelation(13, QSqlRelation(T_TOKENALG, "id", T_TOKENALG_NAME));

    modelCurrencies->select(); // Делаем выборку данных из таблицы
}

void MainWindow::setupRoadmapDatesModel()
{
    modelRoadmapDates = new QSqlRelationalTableModel(this);
    modelRoadmapDates->setTable(T_ROADMAPSDATESPOOL);

    modelRoadmapDates->setRelation(1, QSqlRelation(T_CURRENCIES, "id", T_CURRENCIES_NAME));

    modelRoadmapDates->select();
}

void MainWindow::setupTechPoolModel()
{
    modelTechPool = new QSqlRelationalTableModel(this);
    modelTechPool->setTable(T_TECHPOOL);

    modelTechPool->setRelation(1, QSqlRelation(T_CURRENCIES, "id", T_CURRENCIES_NAME));
    modelTechPool->setRelation(2, QSqlRelation(T_TECHNOLOGIES, "id", T_TECHNOLOGIES_NAME));

    modelTechPool->select();
}

void MainWindow::setupGithubPoolModel()
{
    modelGithubPool = new QSqlRelationalTableModel(this);
    modelGithubPool->setTable(T_GITHUBPOOL);

    modelGithubPool->setRelation(1, QSqlRelation(T_CURRENCIES, "id", T_CURRENCIES_NAME));

    modelGithubPool->select();
}

void MainWindow::setupMarketPoolModel()
{
    modelMarketPool = new QSqlRelationalTableModel(this);
    modelMarketPool->setTable(T_MARKETSPOOL);

    modelMarketPool->setRelation(1, QSqlRelation(T_CURRENCIES, "id", T_CURRENCIES_NAME));

    modelMarketPool->select();
}

void MainWindow::setupTokenAlgModel()
{
    modelTokenAlg = new QSqlRelationalTableModel(this);
    modelTokenAlg->setTable(T_TOKENALG);

    modelTokenAlg->select();
}

void MainWindow::setupAppAreasModel()
{
    modelAppAreas = new QSqlRelationalTableModel(this);
    modelAppAreas->setTable(T_AREAS);

    modelAppAreas->select();
}

void MainWindow::setupTechnologiesModel()
{
    modelTechnologies = new QSqlRelationalTableModel(this);
    modelTechnologies->setTable(T_TECHNOLOGIES);

    modelTechnologies->select();
}
