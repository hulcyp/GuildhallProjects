package c19.hulcy.glprimitivesdemo;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.opengles.GL10;

//Base class for all primitve geometeries to draw. 
//This class handles drawing the VAOs that are set by the derived classes
public class GLPrimitive {
	
	protected FloatBuffer vertexBuffer;
	protected ShortBuffer indexBuffer;
	protected FloatBuffer colorBuffer;
	protected int idxCount;
	protected float lineWidth = 1.0f;
	protected float pointSize = 1.0f;
	
	protected int renderMode = GL10.GL_TRIANGLES;
	
	
	public GLPrimitive()
	{}
	
	public void draw( GL10 gl )
	{
		gl.glFrontFace( GL10.GL_CCW );
		gl.glEnable( GL10.GL_CULL_FACE );
		gl.glCullFace( GL10.GL_BACK );
		
		gl.glLineWidth( lineWidth );
		gl.glPointSize( pointSize );
		
		gl.glEnableClientState( GL10.GL_VERTEX_ARRAY );
		gl.glVertexPointer( 3, GL10.GL_FLOAT, 0, vertexBuffer);
		
		gl.glEnableClientState( GL10.GL_COLOR_ARRAY );
		gl.glColorPointer( 4, GL10.GL_FLOAT, 0, colorBuffer );
		
		
		gl.glDrawElements( renderMode, idxCount, GL10.GL_UNSIGNED_SHORT, indexBuffer );
		
		gl.glDisableClientState( GL10.GL_VERTEX_ARRAY );
		gl.glDisableClientState( GL10.GL_COLOR_ARRAY );
		
		gl.glDisable( GL10.GL_CULL_FACE );
		gl.glLineWidth( 1.0f );
	}
}
