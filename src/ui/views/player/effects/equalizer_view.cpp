#include "ui/views/player/effects/equalizer_view.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include "app.hpp"
#include "ui/helpers/strings_helper.hpp"
#include <iostream>

void EqualizerView::init() {
    root = new QGroupBox("Equalizer");
    auto hlayout = new QHBoxLayout();
    auto hlayoutContainer = new QWidget(root);
    auto vlayout = new QVBoxLayout();

    toggler = new QCheckBox;
    connect(toggler, &QCheckBox::stateChanged, this, &EqualizerView::toggle);
    hlayout->addWidget(toggler);
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->addWidget(new QLabel(tr("Enable/Disable equalization")), 1);
    hlayoutContainer->setLayout(hlayout);
    vlayout->addWidget(hlayoutContainer, 0);

    hlayout = new QHBoxLayout();
    hlayoutContainer = new QWidget(root);
    groupControl = new QCheckBox;
    hlayout->addWidget(groupControl);
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->addWidget(new QLabel(tr("Toggle group control")), 1);
    hlayoutContainer->setLayout(hlayout);
    vlayout->addWidget(hlayoutContainer, 0);

    slidersLayoutContainer = new QWidget(root);
    slidersLayoutContainer->setContentsMargins(0,0,0,0);
    slidersLayoutContainer->setMaximumWidth(1000);
    slidersLayout = new QHBoxLayout();
    slidersLayout->setSpacing(2);
    slidersLayoutContainer->setLayout(slidersLayout);

    vlayout->addWidget(slidersLayoutContainer, 1);

    root->setLayout(vlayout);
}

void EqualizerView::refresh() {
    if (isFirstRefresh) {
        bool isEnabled = app->processors().equalizer().isEnabled();
        QString color1 = "#4444f4";
        QString color2 = "#8888f8";
        QString sliderStylesheet = R"(
            QSlider {
                max-width: 100%;
            }
            QSlider::sub-page:vertical {
                background: #d4d4d4;
            }
            QSlider::groove:vertical {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 )" + color1 + ", stop:1 " + color2 + R"();
            }
            QSlider::handle:vertical {
                background: #aaa;
                border: none;
                height: 10px;
            }
            QSlider::groove:vertical:disabled {
                background: #888
            }
        )";
        for(unsigned int i = 0; i < app->processors().equalizer().bands(); i++) {
            auto& filter = app->processors().equalizer().filter(i);
            auto slider = new QSlider(Qt::Vertical, slidersLayoutContainer);
            auto sliderContainer = new QWidget;
            auto sliderLayout = new QVBoxLayout;
            sliderLayout->setContentsMargins(0,0,0,0);
            slider->setMinimum(0);
            slider->setMaximum(100);
            slider->setValue(100);
            slider->setStyleSheet(sliderStylesheet);
            connect(slider, &QSlider::valueChanged, this, &EqualizerView::updateEqualizer);
            slider->setEnabled(isEnabled);
            sliderLayout->addWidget(slider);
            auto label = new QLabel(QString::fromStdString(StringsHelper::formatFrequency(filter.centralFrequency())));
            label->setAlignment(Qt::AlignHCenter);
            sliderLayout->addWidget(label);
            sliderContainer->setLayout(sliderLayout);
            slidersLayout->addWidget(sliderContainer, 1);
            sliders.push_back(slider);
        }
    } 
}

void EqualizerView::updateEqualizer([[maybe_unused]]int value) {
    for(unsigned int i = 0; i < app->processors().equalizer().bands(); i++) {
        if (groupControl->isChecked()) {
            app->processors().equalizer().filter(i).gain((float)value / 100.0f);
            sliders[i]->blockSignals(true);
            sliders[i]->setValue(value);
            sliders[i]->blockSignals(false);
        } else {
            app->processors().equalizer().filter(i).gain((float)sliders[i]->value() / 100.0f);
        }
    }
}

void EqualizerView::toggle() {
    app->processors().equalizer().toggle();
    bool isEnabled = app->processors().equalizer().isEnabled();
    for (auto slider: sliders) {
        slider->setEnabled(isEnabled);
    }
    groupControl->setEnabled(isEnabled);
}