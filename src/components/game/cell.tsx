import { CellState, CellStateType } from "@/types";
import { BombIcon, FlagTriangleRight, TargetIcon } from "lucide-react";
import { Button } from "../ui/button";
import { useWebSocketContext } from "../contexts/websocket-content";

type Props = {
  x: number;
  y: number;
  disabled: boolean
} & (
  | {
      state: typeof CellState.n;
      number: number;
    }
  | {
      state: Exclude<CellStateType, typeof CellState.n>;
    }
);

export default function Cell(props: Props) {
  const { updateBoard } = useWebSocketContext();

  function markCell(action: "r" | "f") {
    return () => {
      try {
        updateBoard(props.x, props.y, action);
      } catch (err) {
        console.error(err);
      }
    };
  }

  switch (props.state) {
    case CellState.u:
      return <UnreveiledCell markCell={markCell} disabled={props.disabled} />;
    case CellState.f:
      return <FlagCell markCell={markCell} disabled={props.disabled} />;
    case CellState.n: // Check number variant because it comes with props
      return <NumberCell n={props.number} />;
    case CellState.b:
      return <BombCell />;
    case CellState.e:
      return <EmptyCell />;
    case CellState.x:
      return <ExplosionCell />;
  }
}

const number_colors = [
  "text-blue-400",
  "text-green-400",
  "text-red-500",
  "text-blue-500",
  "text-red-700",
  "text-teal-700",
  "text-gray-400",
  "text-orange-900",
] as const;

function UnreveiledCell({
  markCell,
  disabled
}: {
  markCell: (action: "r" | "f") => () => void;
  disabled: boolean
}) {
  return (
    <Button
      onAuxClick={markCell("f")}
      onClick={markCell("r")}
      disabled={disabled}
      className="aspect-square w-8 h-8 p-0 bg-gray-500 border-2 border-gray-800 border-r-0"
    ></Button>
  );
}

function FlagCell({
    markCell,
    disabled
  }: {
    markCell: (action: "r" | "f") => () => void;
    disabled: boolean
  }) {
  return (
    <Button
      onAuxClick={markCell("f")}
      onClick={markCell("r")}
      disabled={disabled}
      className="aspect-square w-8 h-8 p-0 bg-gray-500 flex justify-center items-center"
    >
      <FlagTriangleRight size={20} className="text-black" />
    </Button>
  );
}

function EmptyCell() {
  return <div className="p-0 m-0 aspect-square w-8 h-8 bg-gray-800"></div>;
}

function NumberCell({ n }: { n: number }) {
  return (
    <div className="aspect-square w-8 h-8 bg-gray-800 flex justify-center items-center">
      <span className={`${number_colors[n - 1]}`}>{n}</span>
    </div>
  );
}

function ExplosionCell() {
  return (
    <div className="aspect-square w-10 bg-red-500 flex justify-center items-center">
      <TargetIcon
        size={20}
        className="text-black animate-ping repeat-infinite"
      />
    </div>
  );
}

function BombCell() {
  return (
    <div className="aspect-square w-8 h-8 bg-gray-800 flex justify-center items-center">
      <BombIcon size={20} className="text-red-500" />
    </div>
  );
}
