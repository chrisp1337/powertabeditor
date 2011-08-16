/*
  * Copyright (C) 2011 Cameron White
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
  
#ifndef SCOREAREA_H
#define SCOREAREA_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

#include <painters/musicfont.h>

class StaffData;
class Staff;
class Score;
class System;
class PowerTabDocument;
class TimeSignature;
class Caret;
class Note;
class StdNotationPainter;
class Position;
class TempoMarker;
class AlternateEnding;
class Barline;
class Dynamic;

template <class Key, class T>
class QMultiMap;

// The visual display of the score

class ScoreArea : public QGraphicsView
{
    Q_OBJECT

public:
    explicit ScoreArea(QWidget *parent = 0);

    void renderDocument(boost::shared_ptr<PowerTabDocument> doc);
    void updateSystem(const quint32 systemIndex);

    boost::shared_ptr<PowerTabDocument> document;

    inline Caret* getCaret() const
    {
        return caret;
    }

protected:
    void renderScore(Score* score, int lineSpacing);
    void renderSystem(Score* score, boost::shared_ptr<const System> system, int lineSpacing);

    void drawTabNotes(boost::shared_ptr<const System> system, boost::shared_ptr<const Staff> staff,
                      const StaffData& currentStaffInfo);

    void renderBars(const StaffData& currentStaffInfo, boost::shared_ptr<const System> system);
    void drawTabClef(int x, const StaffData& staffInfo);

    void drawChordText(boost::shared_ptr<const System> system, quint32 height, const StaffData& currentStaffInfo);
    void drawDirections(boost::shared_ptr<const System> system, quint32 height,
                        const StaffData& currentStaffInfo);

    void drawLegato(boost::shared_ptr<const System> system, boost::shared_ptr<const Staff> staff,
                    const StaffData& currentStaffInfo);

    void drawSlides(boost::shared_ptr<const System> system, boost::shared_ptr<const Staff> staff,
                    const StaffData& currentStaffInfo);

    void drawSlidesHelper(boost::shared_ptr<const System> system, const StaffData& currentStaffInfo, quint8 string,
                          bool slideUp, quint32 posIndex1, quint32 posIndex2);

    void drawStdNotation(boost::shared_ptr<const System> system, boost::shared_ptr<const Staff> staff,
                         const StaffData& currentStaffInfo);

    void adjustAccidentals(QMultiMap<int, StdNotationPainter*>& accidentalsMap);
    void drawSystemSymbols(Score* score, boost::shared_ptr<const System> system, const StaffData& currentStaffInfo);
    void drawDividerLine(const StaffData& currentStaffInfo, quint32 y);

    void drawAltEndings(const std::vector<boost::shared_ptr<AlternateEnding> >& altEndings,
                        boost::shared_ptr<const System> system, uint32_t height);

    void drawTempoMarkers(const std::vector<boost::shared_ptr<TempoMarker> >& tempoMarkers,
                          boost::shared_ptr<const System> system, quint32 height);

    void drawSymbolsBelowTabStaff(boost::shared_ptr<const System> system, boost::shared_ptr<const Staff> staff,
                                  const StaffData &currentStaffInfo);

    void drawRhythmSlashes(boost::shared_ptr<const System> system);

    void drawSymbols(const Score* score, boost::shared_ptr<const System> system,
                     boost::shared_ptr<const Staff> staff, const StaffData& staffInfo);

    void drawArpeggio(Position* position, quint32 x, const StaffData& currentStaffInfo);

    QGraphicsScene scene;
    MusicFont musicFont;
    void centerItem(QGraphicsItem* item, float xmin, float xmax, float y) const;
    Caret* caret;
    QList<QGraphicsItem*> systemList;

    // holds the parent object of the staff that is being drawn
    // all QGraphicsItems that are a part of the staff have this object as their parent
    QGraphicsItem* activeStaff;
    QGraphicsItem* activeSystem;
    bool redrawOnNextRefresh;

    // Symbol drawing
    struct SymbolInfo
    {
        QRect rect; ///< the bounding rectangle of the symbol group
        QGraphicsItem* symbol; ///< contains the information for rendering the symbol
    };

    enum VolumeSwellType
    {
        VolumeIncreasing,
        VolumeDecreasing
    };

    QGraphicsItem* createVolumeSwell(uint8_t width, const StaffData& currentStaffInfo, VolumeSwellType type);

    QGraphicsItem* createDynamic(boost::shared_ptr<const Dynamic> dynamic) const;
    QGraphicsItem* createTrill(const StaffData& currentStaffInfo) const;
    QGraphicsItem* createTremoloPicking(const StaffData& currentStaffInfo) const;
    QGraphicsItem* createTremoloBar(uint8_t width, const Position* position) const;

    QGraphicsItem* createPlainText(const QString& text, QFont::Style style);
    QGraphicsItem* createPickStroke(const QString& text);
    QGraphicsItem* createArtificialHarmonicText(Position* position);

    QGraphicsItem* drawContinuousFontSymbols(QChar symbol, int width);
    QGraphicsItem* createConnectedSymbolGroup(const QString& text, QFont::Style style, int width,
                                              const StaffData& currentStaffInfo);

    void drawMultiBarRest(boost::shared_ptr<const System> system, boost::shared_ptr<const Barline> currentBarline,
                          const StaffData& currentStaffInfo, int measureCount);

signals:
    void barlineClicked(int position);

public slots:
    void adjustScroll();
    void requestFullRedraw();

};

#endif // SCOREAREA_H
