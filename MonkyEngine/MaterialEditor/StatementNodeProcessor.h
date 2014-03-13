#pragma once
//---------------------------------------------
//	Author: Preston Hulcy
//	Date: 1/21/2014
//	Description: Each XML node represents a statement or sub-statement. This class is the abstract base class
//		for processing an XML node. Sub-class this class to create a new XML node type.
//
//---------------------------------------------
#include "CommonCoreEngineIncludes.h"
#include "XMLParser.h"
#include "ShaderVariable.h"

namespace Monky
{
	class ShaderGenerator;

	struct StatementData
	{
		StatementData();
		StatementData( const std::string& statement, ShaderVariableType type );
		StatementData( const std::string& statement, ShaderVariableType type, const std::string& variableData );
		ShaderVariableType outputType;
		std::string statement;
		std::string variableData;
	};

	class StatementNodeProcessor
	{
	public:

		StatementNodeProcessor( const std::string& name, ShaderGenerator* generator );
		virtual ~StatementNodeProcessor();

		virtual StatementData ProcessAsRoot( XMLParser& parser, XMLNode* node ) = 0;
		virtual StatementData ProcessAsChild( XMLParser& parser, XMLNode* node ) = 0;
		virtual void ReloadProcessorDataToShaderGenerator(){}

		const std::string& GetName() const { return m_name; }

	protected:
		StatementData ProcessInputNode( XMLParser& parser, XMLNode* inputNode );
		bool ValidListOfVariables( const std::vector< std::string >& data );
		std::string StripDollarSignsFromListOfVariables( const std::vector< std::string >& data );

		bool ValidListOfCommaSeparatedVariables( const std::string& data ); 
		std::string StripDollarSignsFromCommaSeperatedVariables( const std::string& data );

		std::string BuildVariableConstructionFromVariableList( const std::vector< std::string >& dataList, ShaderVariableType& type );
		bool IsVariableAccessingComponentOfVariable( const std::string variable );

		ShaderGenerator* m_shaderGenerator;
		std::string m_name;
	};
}
