package c19.hulcy.glprimitivesdemo;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import javax.microedition.khronos.opengles.GL10;

public class GLPrimitivePoints extends GLPrimitive {
	
	private float vertices[] =
	{
		-1.0f, -1.0f, 0.0f, 
		 1.0f, -1.0f, 0.0f, 
		-1.0f,  1.0f, 0.0f, 
		 1.0f,  1.0f, 0.0f					
	};
	
	private short[] indices = 
	{
			0, 1, 2, 3
	};
	
	float[] colors = 
	{
		1.0f, 0.0f, 0.0f, 1.0f, // vertex 0 is red
		0.0f, 1.0f, 0.0f, 1.0f, // vertex 1 is green
		0.0f, 0.0f, 1.0f, 1.0f, // vertex 2 is blue
		1.0f, 1.0f, 1.0f, 1.0f  // vertex 3 is white	
	};
	
	public GLPrimitivePoints()
	{
		ByteBuffer vbb = ByteBuffer.allocateDirect( vertices.length * 4 );
		vbb.order( ByteOrder.nativeOrder() );
		vertexBuffer = vbb.asFloatBuffer();
		vertexBuffer.put( vertices );
		vertexBuffer.position( 0 );
		
		ByteBuffer ibb = ByteBuffer.allocateDirect( indices.length * 2 );
		ibb.order( ByteOrder.nativeOrder() );
		indexBuffer = ibb.asShortBuffer();
		indexBuffer.put( indices );
		indexBuffer.position( 0 );
		idxCount = indices.length;
		
		
		ByteBuffer cbb = ByteBuffer.allocateDirect( colors.length * 4 );
		cbb.order( ByteOrder.nativeOrder() );
		colorBuffer = cbb.asFloatBuffer();
		colorBuffer.put( colors );
		colorBuffer.position( 0 );
		
		renderMode = GL10.GL_POINTS;
		pointSize = 10.0f;
	}

}
