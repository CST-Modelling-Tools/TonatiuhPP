/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain additional
 ** rights.  These rights are described in the Nokia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Nokia at qt-info@nokia.com.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include <QPainter>
#include <QTextBlock>

#include "CodeEditArea.h"

CodeEditArea::CodeEditArea( QWidget* parent )
:QPlainTextEdit( parent )
{
	connect(this, SIGNAL( cursorPositionChanged() ), this, SLOT( UpdateCurrentLine() ) );
	/*connect(this, SIGNAL( blockCountChanged( int ) ), this, SLOT( UpdateCodeEditAreaWidth( int ) ) );
	connect(this, SIGNAL( updateRequest( QRect, int ) ), m_LineNumberWidget, SLOT( UpdateLineNumberArea( QRect, int ) ) );

*/
	//UpdateCodeEditAreaWidth(0);
	UpdateCurrentLine();
}


CodeEditArea::~CodeEditArea()
{

}

int CodeEditArea::BlockTop( const QTextBlock & block ) const
{
	return (int) blockBoundingGeometry(block).translated(contentOffset()).top();
}

int CodeEditArea::BlockHeight( const QTextBlock & block ) const
{
	return (int) blockBoundingRect(block).height();
}

 void CodeEditArea::UpdateCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if( !isReadOnly() )
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(Qt::yellow).lighter(160);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}
