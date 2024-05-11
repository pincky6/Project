#ifndef STLMODELBUILDER_H
#define STLMODELBUILDER_H

#include "abstractplotworker.h"

#include "xyzplotcalculator.h"
#include "figures.h"

namespace plot_builder{

    class STLModelBuilder: public AbstractPlotWorker
    {
        Q_OBJECT
    public:
        STLModelBuilder();
        STLModelBuilder(const QString&, const QString&,
                          Range, Range, Range,
                          Resolution, Resolution,
                          AbstractPlotWorker* parent = nullptr);
        STLModelBuilder(const STLModelBuilder&) = delete;
        STLModelBuilder(STLModelBuilder&&) noexcept;

        void work() override;
        void connect();
        void start();

        XYZPlotCalculator &getPlotCalculator();

        void setFilename(const QString&);
        void setExpression(const QString&);
        void setResolution(const Resolution&);
        void setCube(const Cube& cube);
    signals:
        void endCalculations();
        void endFileWriting();
        void wrongCalculations();
    private:
        std::unique_ptr<XYZPlotCalculator> plotCalculator_;

        QString filename_;

        Resolution resolution_;

        QString expression_;

        Cube cube_;
    };
}
#endif // STLMODELBUILDER_H
