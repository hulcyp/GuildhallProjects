package c19.hulcy.glprimitivesdemo;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

public class GLPrimitiveDescActivity extends Activity {
	
	
	private String m_primitiveName; 
		
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView( R.layout.gl_desc_layout );
		
		Intent intent = getIntent();
		
		if( intent != null )
		{
			m_primitiveName = intent.getStringExtra( "primitive_selected" );
		}
		
		
		TextView name = (TextView)findViewById( R.id.primitiveName );
		ImageView image = (ImageView)findViewById( R.id.primitiveImage );
		TextView desc = (TextView)findViewById( R.id.primitiveDesc );
		
		name.setText( m_primitiveName );
		setImageAndDesc( m_primitiveName, image, desc );		
	}	
	
	public void onViewButtonClick( View v )
	{
		Intent demoIntent = new Intent( getApplicationContext(), GLPrimitivesDemoViewActivity.class );
		demoIntent.putExtra( "primitive_selected", m_primitiveName );
		
		startActivity( demoIntent );
	}

	
	public void setImageAndDesc( String name, ImageView image, TextView desc )
	{
		int imgRID = 0;
		int descRID = 0;
		
		if( name.equals( "GL_POINTS" ) )
		{
			imgRID = R.drawable.gl_points;
			descRID = R.string.gl_points;
		}
		else if( name.equals( "GL_LINES" ) )
		{
			imgRID = R.drawable.gl_lines;
			descRID = R.string.gl_lines;			
		}
		else if( name.equals( "GL_LINE_STRIP" ) )
		{
			imgRID = R.drawable.gl_line_strip;
			descRID = R.string.gl_line_strip;
		}
		else if( name.equals( "GL_LINE_LOOP" ) )
		{
			imgRID = R.drawable.gl_line_loop;
			descRID = R.string.gl_line_loop;
		}
		else if( name.equals( "GL_TRIANGLES")  )
		{
			imgRID = R.drawable.gl_triangles;
			descRID = R.string.gl_triangles;
		}
		else if( name.equals( "GL_TRIANGLE_STRIP" ) )
		{
			imgRID = R.drawable.gl_triangle_strip;
			descRID = R.string.gl_triangle_strip;
		}
		else if( name.equals( "GL_TRIANGLE_STRIP" ) )
		{
			imgRID = R.drawable.gl_triangle_strip;
			descRID = R.string.gl_triangle_strip;
		}
		else if( name.equals( "GL_TRIANGLE_FAN" ) )
		{
			imgRID = R.drawable.gl_triangle_fan;
			descRID = R.string.gl_triangle_fan;
		}
					
		image.setImageResource( imgRID );
		desc.setText( getResources().getString( descRID ) );
	}
}
