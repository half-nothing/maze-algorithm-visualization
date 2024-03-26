/**********************************************
 * @file GenerateDisplay.h
 * @brief None
 * @author Half_nothing
 * @email Half_nothing@163.com
 * @version 1.0.0
 * @date 2024.03.26
 * @license GNU General Public License (GPL)
 **********************************************/

#ifndef GENERATEDISPLAY_H
#define GENERATEDISPLAY_H

#include <GenerateThread.h>
#include <WidgetDisplay.h>

namespace QT {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class GenerateDisplay;
    }

    QT_END_NAMESPACE

    class GenerateDisplay final : public WidgetDisplay {
        Q_OBJECT

    public:
        explicit GenerateDisplay(QWidget *parent = nullptr);

        ~GenerateDisplay() override;

    public slots:
        void setHeight(int);

        void setWidth(int);

        void generateMaze(MazeGenerateMethod);

    private:
        Ui::GenerateDisplay *ui;
        GenerateThread *thread = nullptr;
        MazeMap result{0, 0};
    };
}

#endif
