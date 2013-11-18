package c19.hulcy.glprimitivesdemo;

import android.app.Activity;
import android.content.Intent;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
public class GLPrimitivesDemoViewActivity extends Activity {
	
	private static final int INVALID_POINTER_ID = -1;
	
	private OpenGLRenderer glRenderer;
		
	private int activePointerID = INVALID_POINTER_ID;
	
	private ScaleGestureDetector scaleDetector;
	private float scaleFactor = 1.0f;
	
	private GLSurfaceView glSurfaceView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Intent intent = getIntent();
		String primitiveName = intent.getStringExtra( "primitive_selected" );
		
		glSurfaceView = new GLSurfaceView( this );

		glRenderer = new OpenGLRenderer( this, primitiveName );
		glSurfaceView.setRenderer( glRenderer );

		
		scaleDetector = new ScaleGestureDetector( getApplicationContext(), new ScaleListener() );
		
		setContentView( glSurfaceView );		
	}
	
	@Override
	public boolean onTouchEvent( MotionEvent ev )
	{
		scaleDetector.onTouchEvent( ev );
		
		final int action = ev.getAction();
		
		switch( action & MotionEvent.ACTION_MASK )
		{
			case MotionEvent.ACTION_DOWN:
			{
				// First finger has touched the screen
				
				final int pointerIndex = ev.getPointerId( 0 );
				final float x = ev.getX();
				final float y = ev.getY();
				
				activePointerID = pointerIndex;
				glRenderer.setLastPos( x, y );
			}
			break;
			
			case MotionEvent.ACTION_POINTER_DOWN:
			{
				// I don't care about other fingers... lol
			}
			break;
			
			case MotionEvent.ACTION_MOVE:
			{
				final int pointerIndex = ev.findPointerIndex( activePointerID );
				final float x = ev.getX( pointerIndex );
				final float y = ev.getY (pointerIndex );
				
				if( !scaleDetector.isInProgress() )
				{										
					//Update the renderer
					glRenderer.applyRotation( x, y );
				}				
			}
			break;
			
			case MotionEvent.ACTION_UP:
			{
				// The last finger has left the building
				activePointerID = INVALID_POINTER_ID;
			}
			break;
			
			case MotionEvent.ACTION_POINTER_UP:
			{
				final int pointerIndex = (ev.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
				final int pointerID = ev.getPointerId( pointerIndex );
				
				final float x = ev.getX( pointerIndex );
				final float y = ev.getY( pointerIndex );
				
				if( pointerID == activePointerID )
				{
					// Our active pointer is the one going up so we need to pick a new one
					final int newPointerIndex = pointerIndex == 0 ? 1 : 0;

					glRenderer.setLastPos( x, y );
					activePointerID = ev.getPointerId( newPointerIndex );
				}
			}
			break;
			
			case MotionEvent.ACTION_CANCEL:
			{
				activePointerID = INVALID_POINTER_ID;
			}
			break;
		}
		
		return true;
	}
	
	private class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener
	{
		@Override
		public boolean onScale( ScaleGestureDetector detector )
		{
			scaleFactor *= detector.getScaleFactor();
			
			scaleFactor = Math.max( 0.1f, Math.min( scaleFactor, 3.0f ) );
			
			glRenderer.setScaleFactor( scaleFactor );
			return true;
		}
	}
}
