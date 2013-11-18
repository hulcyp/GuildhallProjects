package c19.hulcy.glprimitivesdemo;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLU;


public class OpenGLRenderer implements Renderer {
	
	private GLPrimitive primitive;
	
	private float lastPosX = 0;
	private float lastPosY = 0;
	private float scaleFactor = 1.0f;
	private float rotation = 0.0f;
	
	private int screenWidth;
	private int screenHeight;

	public OpenGLRenderer( Activity activity, String primitiveName )
	{
		loadPrimitive( primitiveName );
	}
	
	@Override
	public void onDrawFrame(GL10 gl) {
		
		gl.glClear( GL10.GL_COLOR_BUFFER_BIT );
		
		if( primitive != null )
		{
			gl.glLoadIdentity();
			gl.glTranslatef( 0.0f, 0.0f, -6 );
			gl.glRotatef( rotation, 0.0f, 0.0f, 1.0f );
			gl.glScalef( scaleFactor, scaleFactor, 1.0f );
			primitive.draw( gl );
		}
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		
		screenWidth = width;
		screenHeight = height;
		
		gl.glViewport(0, 0, screenWidth, screenHeight);
		
		gl.glMatrixMode( GL10.GL_PROJECTION );
		
		gl.glLoadIdentity();
		GLU.gluPerspective( gl, 45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f );
		gl.glMatrixMode( GL10.GL_MODELVIEW );
		
		gl.glLoadIdentity();
		
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		gl.glClearColor( 0.5f, .5f, 0.5f, 1.0f);	
	}
	
	public void setLastPos( float x, float y )
	{
		lastPosY = x;
		lastPosX = x;
	}
	
	public void applyRotation( float x, float y )
	{
		float hWidth = screenWidth * 0.5f;
		float hHeight = screenHeight * 0.5f;
		float dx = x - lastPosX;
		float dy = y - lastPosY;
		
		
		//Check to see which way we should rotate based on the touch location and the direction of the drag
		if( dx > 0 && y > hHeight )
			rotation += dx;
		else if( dx > 0 )
			rotation -= dx;
		else if( dx < 0 && y < hHeight )
			rotation -= dx;
		else
			rotation += dx;
		
		if( dy > 0 && x > hWidth )
			rotation -= dy;
		else if( dy > 0 )
			rotation += dy;
		else if( dy < 0 && y < hWidth )
			rotation += dy;
		else
			rotation -= dy;
		
		lastPosX = x;
		lastPosY = y;
		
	}
	
	public void setScaleFactor( float scale )
	{
		scaleFactor = scale;
	}
	
	
	private void loadPrimitive( String primitiveName )
	{
		//Instantiate an instance of the correct Primitive class based on the provided string
		if( primitiveName.equals( "GL_POINTS" ) )
		{
			primitive = new GLPrimitivePoints();
		}
		else if( primitiveName.equals( "GL_LINES" ) )
		{
			primitive = new GLPrimitiveLines();
		}
		else if( primitiveName.equals( "GL_LINE_STRIP" ) )
		{
			primitive = new GLPrimitiveLineStrip();
		}
		else if( primitiveName.equals( "GL_LINE_LOOP" ) )
		{
			primitive = new GLPrimitiveLineLoop();
		}
		else if( primitiveName.equals( "GL_TRIANGLES" ) )
		{
			primitive = new GLPrimitiveTriangles();
		}
		else if( primitiveName.equals( "GL_TRIANGLE_STRIP" ) )
		{
			primitive = new GLPrimitiveTriangleStrip();
		}
		else if( primitiveName.equals( "GL_TRIANGLE_FAN" ) )
		{
			primitive = new GLPrimitiveTriangleFan();
		}
	}

}
