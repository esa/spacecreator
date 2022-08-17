run Simulink_DataView_asn;

inports_positions  = zeros(1, 4);
bussel_positions   = zeros(1, 4);
outports_positions = zeros(1, 4);
buscre_positions   = zeros(1, 4);

n_inports  = 1 + 1 + 1;
n_outports = 1;

all_inports_created  = false;
all_outports_created = false;

if (exist('Calculate', 'file') == 4)
   load_system('Calculate');
   inportHan = find_system('Calculate','FindAll','on', 'SearchDepth', 1, 'BlockType','Inport');
   outportHan = find_system('Calculate','FindAll','on', 'SearchDepth', 1, 'BlockType','Outport');
   all_inports_created  = (length(inportHan) == n_inports);
   all_outports_created = (length(outportHan) == n_outports);
   % ---------------------------------------------------------------------------------
   % start by removing the Bus Selectors / then lines / finally ports
   % ---------------------------------------------------------------------------------
   % get the handles of all the lines connected to inports
   for i=1:length(inportHan)
      inports_positions(i,:) = get_param(inportHan(i),'Position'); % remember Inport's position
      line_structsIn(i)=get_param(inportHan(i),'LineHandles'); % get the structures
      inLinesHan(i)=line_structsIn(i).Outport;        % get the line connected to the block's Outport
      if (inLinesHan(i) ~= -1) % if exists
         dstBlock = get_param(inLinesHan(i),'DstBlockHandle'); % get the destination block's handle
         if (strcmp(get_param(dstBlock,'BlockType'),'BusSelector'))
            bussel_positions(i,:) = get_param(dstBlock,'Position'); % remember Bus Selector's position
            blockLineStructs = get_param(dstBlock,'LineHandles'); % get the line connected structures
            blockLineHandles = blockLineStructs.Outport; % get the line handlers connected to the bus's outports
            for j=1:length(blockLineHandles)
               if (blockLineHandles(j) ~= -1)
                  delete(blockLineHandles(j));
               end
            end
            delete_block(dstBlock); % delete it if it is a Bus Selector Block
         end
         delete(inLinesHan(i)); % delete the respective line
      end
      delete_block(inportHan(i)); % delete the outdated inport block
   end
   % now remove the outports
   for i=1:length(outportHan)
      outports_positions(i,:) = get_param(outportHan(i),'Position'); % remember Outport's position
      line_structsOut(i)=get_param(outportHan(i),'LineHandles'); % get the structures
      outLinesHan(i)=line_structsOut(i).Inport;       % get the line connected to the block's Inport
      if (outLinesHan(i) ~= -1) % if exists
         srcBlock = get_param(outLinesHan(i),'SrcBlockHandle'); % get the source block's handle
         if (strcmp(get_param(srcBlock,'BlockType'),'BusCreator'))
            buscre_positions(i,:) = get_param(srcBlock,'Position'); % remember Bus Creator's position
            blockLineStructs = get_param(srcBlock,'LineHandles'); % get the line connected structures
            blockLineHandles = blockLineStructs.Inport; % get the line handlers connected to the bus's outports
            for j=1:length(blockLineHandles)
                    if (blockLineHandles(j) ~= -1)
                            delete(blockLineHandles(j));
                    end
            end
            delete_block(srcBlock); % delete it if it is a Bus Creator Block
         end
         delete(outLinesHan(i)); % delete the respective line
      end
      delete_block(outportHan(i)); % delete the outdated outport block
   end
else
   new_system('Calculate');
   load_system('Calculate');
end

% These are the parameters for this Simulink function:
   % Name: intparam
   % Direction: PARAM_IN
   % Name: realparam
   % Direction: PARAM_IN
   % Name: boolparam
   % Direction: PARAM_IN
   % Name: outparam
   % Direction: PARAM_OUT

inport_index = 0;
inport_index = inport_index + 1;
add_block('simulink/Sources/In1', 'Calculate/intparam');
if all_inports_created
   set_param('Calculate/intparam','Position', inports_positions(inport_index,:));
else
   set_param('Calculate/intparam','Position',[25 (inport_index * 100) 55 ((inport_index * 100) + 14)]);
end
set_param('Calculate/intparam','BusOutputAsStruct','on');
set_param('Calculate/intparam','OutDataTypeStr','MyInteger');
inport_index = inport_index + 1;
add_block('simulink/Sources/In1', 'Calculate/realparam');
if all_inports_created
   set_param('Calculate/realparam','Position', inports_positions(inport_index,:));
else
   set_param('Calculate/realparam','Position',[25 (inport_index * 100) 55 ((inport_index * 100) + 14)]);
end
set_param('Calculate/realparam','BusOutputAsStruct','on');
set_param('Calculate/realparam','OutDataTypeStr','MyReal');
inport_index = inport_index + 1;
add_block('simulink/Sources/In1', 'Calculate/boolparam');
if all_inports_created
   set_param('Calculate/boolparam','Position', inports_positions(inport_index,:));
else
   set_param('Calculate/boolparam','Position',[25 (inport_index * 100) 55 ((inport_index * 100) + 14)]);
end
set_param('Calculate/boolparam','BusOutputAsStruct','on');
set_param('Calculate/boolparam','OutDataTypeStr','MyBool');

outport_index = 0;
outport_index = outport_index + 1;
add_block('simulink/Sinks/Out1','Calculate/outparam');
if all_outports_created
   set_param('Calculate/outparam','Position', outports_positions(outport_index,:));
else
   set_param('Calculate/outparam','Position',[430 (outport_index * 100) 460 ((outport_index * 100) + 14)]);
end
set_param('Calculate/outparam','OutDataTypeStr','MyReal');

set_param('Calculate','SaveOutput','off');
set_param('Calculate','SignalLogging','off');
set_param('Calculate','SaveTime','off')
set_param('Calculate','Solver','FixedStepDiscrete');
set_param('Calculate','SystemTargetFile','ert.tlc');
set_param('Calculate','TemplateMakefile','ert_default_tmf');
set_param('Calculate', 'PostCodeGenCommand', 'packNGo(buildInfo);');
set_param('Calculate','StrictBusMsg','ErrorLevel1')
save_system('Calculate');
close_system('Calculate');
