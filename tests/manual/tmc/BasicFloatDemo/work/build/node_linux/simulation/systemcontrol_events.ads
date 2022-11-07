
with Simulation_DataView; use Simulation_Dataview;

with System_Dataview;  use System_Dataview;   -- definition of the PID type

with Basicfloatdemo_Dataview; use Basicfloatdemo_Dataview;
with Basicfloatdemo_Dataview; use Basicfloatdemo_Dataview;


with Basicfloatdemo_Dataview; use Basicfloatdemo_Dataview;
with Basicfloatdemo_Dataview; use Basicfloatdemo_Dataview;

with Systemcontrol_PI;

package Systemcontrol_Events is

   procedure Startup (Global_State : in out asn1SccSystem_State)
      renames Systemcontrol_PI.Startup;

   --  Barrier for the evaluation of continuous signals when Check_Queue is called
   --  Value true ensures that they are not evaluated automatically after the
   --  execution of a transition (it means: there is a message in the queue, and
   --  they have priority). When user in the simulator send the "Input none"
   --  event, it is sent to false, so that continuous signals are evaluated on demand
   --  allowing to address all cases (leaving time to the user or model checker to
   --  place a message in the queue and skip continuous signals).
   CS_Barrier : Boolean := True;

   --  Process event: execute the provided interface for Input events
   --  Only report output events
   procedure Process_Event (Event        :        asn1sccObservable_Event;
                            Global_State : in out asn1SccSystem_State);

   procedure Print_Event (Event : asn1sccSystemcontrol_Event);

end Systemcontrol_Events;
