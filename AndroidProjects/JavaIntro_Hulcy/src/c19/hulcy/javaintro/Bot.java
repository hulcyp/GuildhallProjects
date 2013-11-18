package c19.hulcy.javaintro;

public class Bot {
	private int m_speed;
	
	public Bot( int speed )
	{
		m_speed = speed;
	}
	
	public int GetSpeed()
	{
		return m_speed;		
	}
	
	public void SetSpeed( int speed )
	{
		m_speed = speed;
	}
}
