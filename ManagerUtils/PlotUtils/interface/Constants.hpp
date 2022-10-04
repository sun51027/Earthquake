/*******************************************************************************
*
*  Filename    : Constants.hpp
*  Description : Plotting configuration constants
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_PLOTUTILS_CONSTANTS_HPP
#define MANAGERUTILS_PLOTUTILS_CONSTANTS_HPP

/*******************************************************************************
*   Default canvas settings
*******************************************************************************/
#define DEFAULT_CANVAS_WIDTH   650
#define DEFAULT_CANVAS_HEIGHT  500

/*******************************************************************************
*   Default padding settings
*******************************************************************************/
#define PLOT_X_MIN 0.13
#define PLOT_Y_MIN 0.105
#define PLOT_X_MAX 0.95
#define PLOT_Y_MAX 0.9

/*******************************************************************************
*   Font setting
*******************************************************************************/
#define FONT_TYPE 43// Sans + absolute font size
#define AXIS_TITLE_FONT_SIZE 22
#define AXIS_LABEL_FONT_SIZE 18
#define TEXT_FONT_SIZE 18

/*******************************************************************************
*   Text alignment settings
*******************************************************************************/
#define BOTTOM_LEFT   11
#define BOTTOM_RIGHT  31
#define TOP_LEFT      13
#define TOP_RIGHT     33

#define TEXT_MARGIN      0.04
#define TEXT_HEIGHT      ( double(TEXT_FONT_SIZE) / double(DEFAULT_CANVAS_HEIGHT) )
#define REL_LINE_HEIGHT  ( 1.5 )
#define LINE_HEIGHT      ( TEXT_HEIGHT * REL_LINE_HEIGHT )
#define PLOT_X_TEXT_MIN  ( ( PLOT_X_MIN ) + ( TEXT_MARGIN ) )
#define PLOT_X_TEXT_MAX  ( (PLOT_X_MAX)-( TEXT_MARGIN ) )
#define PLOT_Y_TEXT_MIN  ( ( PLOT_Y_MIN ) + ( TEXT_MARGIN ) )
#define PLOT_Y_TEXT_MAX  ( (PLOT_Y_MAX)-( TEXT_MARGIN ) )

/*******************************************************************************
*   Label plotting
*******************************************************************************/
#define PRELIMINARY 0
#define SIMULATION  1
#define WORK 2
#define GEN 3
#define PUBLISH     -1

#endif/* end of include guard: MANAGERUTILS_PLOTUTILS_CONSTANTS_HPP */
