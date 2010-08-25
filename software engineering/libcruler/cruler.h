
/** @file
 * This file declares CRuler class
 */

#include <windows.h>
#include <vector>

#ifndef _cruler_h
#define _cruler_h

/** @mainpage
 *  This manual documents the CRuler C++ API.\n
 *
 * CRuler is a window control element, which allows user to set different display parameters:
 *  - left and right work space borders 
 *  - set several tabulation parameters 
 * 
 * Also, CRuler allows to save & load its parameters. \n
 *  \n
 *  \n
 *  @author Fedor Zentsev, 3057/2
 */

namespace se_task
{
   /**
    * @struct CrulerSettings
    * Internal storage for CRuler parameters.
    */
   struct CrulerSettings
   {
      int rightBorder,        /**< Right border for the work space, measured in 1/4 centimeters */
          leftBorder;         /**< Left border for the work space, measured in 1/4 centimeters */
      int mouseHooked;        /**< Equals 1 if mouse hooked for left border, -1 otherwise */
      double screenUnit;      /**< Depicts how many pixels contain 1/4 of logical centimeter */
      std::vector<int> tabs;  /**< Tabulation parameters storage, all of them measured in 1/4 centimeters */
   };


   /**
    * @struct CrulerSettings
    * Additional storage for CRuler parameters, used to pass parameters to parent window.
    */
   struct SettingsBuf
   {
      double realRightBorder,   /**< Right border for the work space, measured centimeters */
             realLeftBorder;    /**< Right border for the work space, measured centimeters */
      std::vector<double> tabs; /**< Tabulation parameters storage, all of them measured in centimeters */
   };

   /** CRuler representation type.
    */
   class Cruler
   {
   public:

      /** CRuler messages enum type.
       * An enum type which represents all messages, posted from CRuler window
       */
      enum
      {
         CR_TAB_ADDED = WM_APP,   /**<  The @e CR_TAB_ADDED message is posted to parent window, when
                                   *    new tabulation parameter has been added to CRuler \n\n
                                   *    @e LOWORD(lParam) - tabulation value in pixels \n
                                   *    @e HIWORD(lParam) - tabulation value in logical units  \n
                                   *    @e wParam - screenUnit value
                                   * @see CrulerSettings
                                   * @see logic2cent
                                   * @remarks logic2cent function is used to convert logic units in centimeters 
                                   */
         CR_RBORDER_CHANGED,      /**<  The @e CR_RBORDER_CHANGED message is posted to parent window, when
                                   *    right CRuler border has changed \n\n
                                   *    @e LOWORD(lParam) - right border value in pixels \n
                                   *    @e HIWORD(lParam) - right border in logical units  \n
                                   *    @e wParam - screenUnit value
                                   * @see CrulerSettings
                                   * @see logic2cent
                                   * @remarks logic2cent function is used to convert logic units in centimeters 
                                   */
         CR_LBORDER_CHANGED,      /**<  The @e CR_LBORDER_CHANGED message is posted to parent window, when
                                   *    left CRuler border has changed \n\n
                                   *    @e LOWORD(lParam) - left border value in pixels \n
                                   *    @e HIWORD(lParam) - left border in logical units  \n
                                   *    @e wParam - screenUnit value
                                   * @see CrulerSettings
                                   * @see logic2cent
                                   * @remarks logic2cent function is used to convert logic units in centimeters 
                                   */
      };

      /** CRuler constants enum type
       */
      enum
      {
         CRULER_OFFSET = 5,       /**< @e The CRULER_OFFSET constant represents number of 
                                    *     pixels from the top of parent window */
         CRULER_HEIGHT = 30,      /**< @e The CRULER_HEIGHT constant represents height of CRuler window */
         CRULER_CENTIMETERS = 21  /**< @e CRULER_CENTIMETERS constant represents centimeters count  */
      };

      /**
       * Default CRuler constructor.
       * It initializes @e _hWnd and @e _hParentWnd by NULL.
       * Creates font for digits in CRuler window.
       * Initializes @e left and @e right borders to 0.25 centimeters (1 logical unit)
       */
      Cruler();

      /**
       * Window attaching function.
       * Creates child CRuler window for the parent window.
       * Registers new window class with @e CRULER_CLASS_NAME name
       * @param _hMainWnd handle to parent window
       * @throw Exception \n
       *   - invalid window handle passed
       *   - creating CRuler window failed
       *   - trying to attach CRuler, which is already attached  
       */
      void attach( HWND _hMainWnd );

      /**
       * Window reattaching function.
       * Detaches window from previous parent window and attaches to the next
       * @param _hMainWnd handle to parent window
       * @throw Exception \n
       *   - invalid window handle passed
       *   - creating CRuler window failed
       *   - trying to attach CRuler, which is already attached  
       */
      void reattach( HWND _hMainWnd );
     
      /**
       * Window detaching function.
       * Detaches window from parent window
       * @throw Exception \n
       *   - trying to detach already detached CRuler
       */
      void detach();

      /**
       * Updates the size of CRuler window
       */
      void resize();


      /**
       * Gets CRulers current settings buffer
       * @see SettingsBuf
       */
      void getState( SettingsBuf &buf );

      /**
       * Sets CRulers current settings buffer
       * @see SettingsBuf
       */
      void setState( const SettingsBuf &buf );

      /**
       * Auxiliary function for converting.  
       * @param logic_var value measured in CRuler logical units, less than @e CRULER_CENTIMETERS * 4  
       * @return value converted in centimeters
       */
      static double logic2cent( int logic_var );

      /**
       * Auxiliary function for converting.  
       * @param cent_var value measured in centimeters, less than @e CRULER_CENTIMETERS  
       * @return value converted in logical units
       */
      static int cent2logic( double cent_var );

      /**
       * Default CRuler destructor. Detaches CRuler window and clears tabulation array.
       */
      ~Cruler();

   private:

      static LRESULT CALLBACK _wndProc( HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam );
      static void _crulerOnPaint( HWND hWnd );
      static void _crulerOnSize( HWND hWnd, UINT state, int cx, int cy );
      static void _crulerOnMouseMove( HWND, int x, int y, UINT keyFlags );
      static void _crulerOnLButtonDown( HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags ); 
      static void _crulerOnLButtonUp( HWND hwnd, int x, int y, UINT keyFlags ); 

      void _draw( HWND hWnd, HDC hDC );
      void _addTab( int tab );
      void _updateBorders();
      
      void _registerClass();

      HWND _hWnd;
      HWND _hParentWnd;

      CrulerSettings _state;
   };
}

#endif /* _cruler_h */