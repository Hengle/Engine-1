#pragma once

#include "Application/API.h"
#include "Application/Inspect/DragDrop/FilteredDropTarget.h"
#include "Application/Inspect/Reflect/ReflectFieldInterpreter.h"

namespace Finder
{
  class FinderSpec;
}

namespace Inspect
{
  class APPLICATION_API FileInterpreter : public ReflectFieldInterpreter
  {
  public:
    FileInterpreter (Container* container);

    virtual void InterpretField(const Reflect::Field* field, const std::vector<Reflect::Element*>& instances, Container* parent);

  private:

    // callbacks
    bool DataChanging( DataChangingArgs& args );
    void Edit( Button* button );
    
    void OnDrop( const Inspect::FilteredDropTargetArgs& args );

  protected:
      std::string m_FileFilter;

  private:
    Inspect::Value* m_Value;
  };

  typedef Nocturnal::SmartPtr<FileInterpreter> FileInterpreterPtr;
  typedef std::vector< FileInterpreterPtr > V_FileInterpreter;
}